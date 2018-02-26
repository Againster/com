function inherit (superclass, subclass) {
    subclass.prototype = Object.create(superclass.prototype);
    subclass.prototype.constructor = subclass;
}

function Subject () {
    this.observers = new Array();

    Subject.prototype.attach = function (observer) {
        var index = this.observers.length;
        this.observers[index] = observer;
    };

    Subject.prototype.detach = function (observer) {
        for (var i = 0; i < this.observers.length; i++) {
            if (this.observers[i] === observer) {
                delete (this.observers[i]);
            }
        }
    };

    Subject.prototype.notify = function (key, value) {
        for (var i = 0; i < this.observers.length; i++) {
            if (this.observers[i] &&
                typeof this.observers[i].update === 'function') {
                this.observers[i].update(key, value);
            }
        }
    };
};

inherit(Subject, ConcreteSubject);
function ConcreteSubject () {
    Subject.call(this);
}


inherit(Subject, ConcreteSubjectX);
function ConcreteSubjectX () {
    Subject.call(this);
}



function Observer () {
    if (this.constructor === Observer) {
        throw new Error("Cannot instanciate abstract class");
    }

    Observer.prototype.update = function () {
        throw new Error("Cannot call abstract method");
    };
};


inherit(Observer, ConcreteObserver);
function ConcreteObserver () {
    Observer.call(this, arguments);

    ConcreteObserver.prototype.update = function (key, value) {
        console.log("hi--" + key + ":" + value);
    }
}


inherit(Observer, ConcreteObserverX);
function ConcreteObserverX () {
    Observer.call(this, arguments);

    ConcreteObserverX.prototype.update = function (key, value) {
        console.log("XXX--" + key + ":" + value);
    }
}

var cs = new ConcreteSubject();
var csx = new ConcreteSubjectX();
var co = new ConcreteObserver();
var co1 = new ConcreteObserver();
var co2 = new ConcreteObserverX();
csx.attach(co);
cs.attach(co);
cs.attach(co1);
cs.attach(co2);
/*
cs.notify("hello", "world");
cs.detach(co);
cs.notify("hellolll", "world");
*/

csx.notify("csx", "but");
csx.detach(co);
csx.notify("csx", "hsdkf");

