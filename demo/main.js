import createMidier from '../dist/Midier.js'

async function main() {
    console.log("Initializing Midier");
    const midier = await createMidier();

    console.log("Creating a Midier sequencer");
    const sequencer = new midier.Sequencer();

    console.log("Registering key press handlers");
    var layers = {};
    var bpm = midier.getBPM();
    document.addEventListener('keydown', event => {
        if (event.code == "Equal" && bpm < 220) {
            bpm += 1;
            midier.setBPM(bpm);
        }
        else if (event.code == "Minus" && bpm > 20) {
            bpm -= 1;
            midier.setBPM(bpm);
        } else {
            if (event.key in layers) {
                return;
            }

            if ('12345678'.indexOf(event.key) >= 0) {
                layers[event.key] = sequencer.start(parseInt(event.key, 10));
            }
        }
    });
    document.addEventListener('keyup', event => {
        if (event.key in layers) {
            sequencer.stop(layers[event.key]);
            delete layers[event.key];
        }
    });

    console.log("Initializing Web MIDI");
    const midiAccess = await navigator.requestMIDIAccess();
    var id = null;
    midiAccess.outputs.forEach(output => {
        // console.log(output);
        id = output.id;
    });

    if (id === null) {
        console.log("Could not find a MIDI output")
    } else {
        var midiOutput = midiAccess.outputs.get(id);

        console.log("Registering MidierJS MIDI handlers");

        midier.onMIDINoteOn((number, velocity) => {
            midiOutput.send([0x90, number, velocity]);
        });

        midier.onMIDINoteOff((number) => {
            midiOutput.send([0x80, number, 127]);
        });
    }

    console.log("Playing a layer");
    await sequencer.play(1, 1);
    console.log("Done");
}

main().then({}).catch(err => { console.error(err); })
