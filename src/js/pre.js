Module["onRuntimeInitialized"] = async _ => {
    // MIDI handlers
    Module["onMIDINoteOn"] = function(handler) {
        const p = Module["addFunction"](handler, 'vii');
        Module.ccall('onMIDINoteOn', null, ['number'], [p]);
    };

    Module["onMIDINoteOff"] = function(handler) {
        const p = Module["addFunction"](handler, 'vi');
        Module.ccall('onMIDINoteOff', null, ['number'], [p]);
    };

    // Global configuration
    Module["getBPM"] = Module.cwrap('getBPM', 'number', []);
    Module["setBPM"] = Module.cwrap('setBPM', null, ['number']);

    // Run, schedule and callbacks
    Module["__scheduleCallback"] = Module.cwrap('scheduleCallback', null, ['number', 'number']);

    Module["schedule"] = function(callback, bars) {
        const p = Module["addFunction"](() => {
            Module["removeFunction"](p);
            callback();
        }, 'v');

        Module['__scheduleCallback'](p, bars);
    };

    Module["run"] = async function(bars) {
        await new Promise(resolve => {
            Module["schedule"](resolve, bars);
        });
    }

    // Sequencer
    Module["__createSequencer"] = Module.cwrap('createSequencer', 'number', ['number'])
    Module["__startLayer"] = Module.cwrap('startLayer', 'number', ['number', 'number'])
    Module["__stopLayer"] = Module.cwrap('stopLayer', null, ['number'])

    Module["Sequencer"] = class {
        constructor(layers = 48) {
            this._sequencer = Module['__createSequencer'](layers);
        }

        start(degree) {
            return Module["__startLayer"](this._sequencer, degree);
        }

        stop(handle) {
            Module["__stopLayer"](handle);
        }

        async play(degree, bars) {
            const layer = this.start(degree);
            await Module["run"](bars);
            this.stop(layer);
        }
    };
};
