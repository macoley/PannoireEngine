
/**
 * Own Module system based on Node.js Module system
 * See: http://fredkschott.com/post/2014/06/require-and-the-module-system/
 * See: https://github.com/nodejs/node/blob/master/lib/internal/modules/cjs/loader.js
 */

this.LoadedModules = {};
this.RootModule = new Module("");
 
function Module(path) {
    this.path = path;
    this.exports = {};
    this.parents = [];
    this.children = [];

    this.toJSON = function() {
        return {'using': this.children.length, 'used_by': this.parents.length};
    }
}

Module.prototype._resolveFileName = function(request) {
    var pos_lastSlash = request.lastIndexOf('/');
    var fileName = request.substr(pos_lastSlash + 1);
    return fileName;
}

Module.prototype._resolveModuleName = function(request) {
    var fileName = Module._resolveFileName(request);
    var pos_firstDot = fileName.indexOf('.');
    var moduleName;

    if(pos_firstDot == -1)
        moduleName = fileName;
    else
        moduleName = fileName.substr(0, pos_firstDot)

    return moduleName;
}

Module.prototype._hasExtension = function(request) {
    return (request.indexOf('.') != -1);
}

Module.prototype._isAbsolutePath = function(request) {
    return request.startsWith('/');
}

Module.prototype._clearPath = function(request) {
    var path = request.trim();
    path = path.replace("./", '');

    // "dir/../" -> "/"
    while(path.search(/\w+\/\.\.\//g) != -1)
        path = path.replace(/\w+\/\.\.\//g, '/');

    // "//" -> "/"
    while(path.includes("//"))
        path = path.replace("//", "/");

    return path;
}

Module.prototype._resolveDirectory = function(request) {
    var pos_lastSlash = request.lastIndexOf('/');
    return request.substr(0, pos_lastSlash + 1);
}

Module.prototype._resolvePath = function(request, parent) {
    var resultPath;

    if(this._isAbsolutePath(request))
        resultPath = this._clearPath(request);
    else
    {
        var parentDirectory = this._resolveDirectory(parent.path);
        resultPath = this._clearPath(parentDirectory + request);
    }

    var exts = [
        '.d.ts',
        '.ts'
    ];

    if(!this._hasExtension(resultPath)) {
        for (var i = 0; i < exts.length; i++) {
            var path = resultPath + exts[i];

            if(ifFileExists(path))
                return path;
        }
    }

    if(ifFileExists(resultPath))
        return resultPath;

    return null;
}

Module.prototype._addParent = function(parent) {
    if(!parent.children.includes(this))
        parent.children.push(this);

    if(!this.parents.includes(parent))
        this.parents.push(parent); 
}

Module.prototype._removeParent = function(parent) {
    var myIndex = parent.children.indexOf(this);
    if (myIndex > -1) {
        parent.children.splice(myIndex, 1);
    }

    var parentIndex = this.parents.indexOf(parent);
    if (parentIndex > -1) {
        this.parents.splice(parentIndex, 1);
    }
}

Module.prototype._compile = function(source) {
    return '(function (exports, require, module, __filename, __dirname) { ' + source + ' })';
}

Module.prototype._transpile = function(source) {
    return TypeScript.ts.transpile(source, {'compilerOptions':{'module': 'es5', 'target': 'es5'}});
}

Module.prototype._load = function(request, parent) {
    var path = this._resolvePath(request, parent);

    if(!path) {
        log("Error! " + request + " doesn't exist!");
        return null;
    }

    // Check if a module already exists in the cache
    var cachedModule = LoadedModules[path];
    if(cachedModule)
    {
        cachedModule._addParent(parent);
        return cachedModule.exports;
    }

    // Otherwise load module
    var createdModule = new Module(path);
    createdModule._addParent(parent);
    LoadedModules[path] = createdModule;
    var source = loadFileContent(path);
    var compiled = this._compile(this._transpile(source));    
    var fnc = eval(compiled);

    //log(compiled);

    var thatModule = this;
    var requireFnc = function(_request) {
        return thatModule._load(_request, createdModule).exports;
    }

    fnc(createdModule.exports, requireFnc, createdModule, this._resolveFileName(path), this._resolveDirectory(path));
 
    return createdModule;
}

function loadModule(request) {
    return RootModule._load(request, RootModule).path;
}

function unloadModule(request) {
    var path = RootModule._resolvePath(request, RootModule);

    if(!path) {
        log("Error! " + request + " doesn't exist!");
        return null;
    }

    var cachedModule = LoadedModules[path];
    if(cachedModule)
        cachedModule._unload();
}

Module.prototype._unload = function() {
    // Remove me from my parents
    for (var i = 0; i < this.parents.length; i++) {
        var parent = this.parents[i];

        this._removeParent(parent);
    }

    // Remove me from my children
    for (var i = 0; i < this.children.length; i++) {
        var child = this.children[i];

        child._removeParent(this);

        if(child.parents.length == 0)
        {
            child._unload();
        }
    }

    delete LoadedModules[this.path];
}

/**
 * Instatiate modules
 */

this.Instances = {
    _indexCounter: 0,
    _freeIndexes: [],
    data: []
};

function generateIndex() {
    if(Instances._freeIndexes.length > 0)
        return Instances._freeIndexes.pop();

    return Instances._indexCounter++;
}

function applyAndNew(constructor, args) {
    function partial () {
       return constructor.apply(this, args);
    };
    if (typeof constructor.prototype === "object") {
       partial.prototype = Object.create(constructor.prototype);
    }
    return partial;
 }
 
 /**
  * Instatiate module property
  * @param {String} path 
  * @param {String} property 
  * @param {Array} args 
  */
 
function instantiate(path, property, args) {
    var cachedModule = LoadedModules[path];

    if(cachedModule)
    {
        var index = generateIndex();

        Instances.data[index] = applyAndNew(cachedModule.exports[property], args);
        return index;
    }
}