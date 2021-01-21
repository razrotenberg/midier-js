import createMidier from '../../dist/Midier.js'
import {CodeJar} from 'codejar';
import './style.css';
import hljs from 'highlight.js/lib/core';

// add JS support only
hljs.registerLanguage('javascript', require('highlight.js/lib/languages/javascript'));

async function main() {
    console.log("Initializing editor");
    const jar = CodeJar(document.querySelector('#editor'), (editor) => {
        // highlight.js does not trims old tags,
        // let's do it by this hack.
        editor.textContent = editor.textContent;
        hljs.highlightBlock(editor);
    });

    jar.updateCode(
`// for every scale degree
for (const index of Array(8).keys()){
    const degree = index + 1

    // play the scale degree for one bar
    await sequencer.play(degree, 1);
}`);

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

        // register the click handler for the 'run' button
        // now that we have everything set up
        document.querySelector("#run").onclick = function() {
            eval("(async () => {" + jar.toString() + "})()");
        }
    }
}

main().then({}).catch(err => { console.error(err); })
