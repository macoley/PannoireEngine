
#include "PE/Resource/WindowsFileMonitor.h"

namespace PE::Resource {

    void WindowsFileMonitor::watchDirectory(const std::string &path, IFileMonitor::CallbackFnc callback) {

        m_watchers.emplace(path, [&]() {
            HANDLE hDirectory = CreateFile(path.c_str(),
                                           FILE_LIST_DIRECTORY,
                                           FILE_SHARE_READ | FILE_SHARE_DELETE,
                                           NULL,
                                           OPEN_EXISTING,
                                           FILE_FLAG_BACKUP_SEMANTICS,
                                           NULL);

            const int MAX_BUFFER = 1024;
            FILE_NOTIFY_INFORMATION Buffer[MAX_BUFFER];
            DWORD dwBytesReturned = 0;

            std::string last_file;
            auto last_time = std::chrono::system_clock::now();

            while(true) {
                ReadDirectoryChangesW(hDirectory,
                                      Buffer,
                                      MAX_BUFFER,
                                      TRUE,
                                      FILE_NOTIFY_CHANGE_LAST_WRITE,
                                      &dwBytesReturned,
                                      0,
                                      0);

                std::basic_string<wchar_t> filename(Buffer->FileName, Buffer->FileNameLength/2);
                std::string filename_string(filename.begin(), filename.end());

                auto new_time = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = new_time - last_time;

                if(last_file != filename_string || elapsed_seconds.count() > 0.5f)
                    callback(path + "/" + filename_string);

                last_file = filename_string;
                last_time = new_time;
            }

        });

        m_watchers[path].detach();

    }

}
