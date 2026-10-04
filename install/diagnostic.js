// GLYPH Diagnostic installer: a serial monitor (Web Serial) to read what
// the diagnostic firmware reports after it is installed.

let port;
let reader;
let inputDone;
let outputDone;
let inputStream;
let outputStream;

const terminal = document.getElementById('terminal');
const connectBtn = document.getElementById('connectSerialBtn');
const connectBtnText = document.getElementById('connectBtnText');
const connectStatusDot = document.getElementById('connectStatusDot');
const clearBtn = document.getElementById('clearBtn');
const serialInput = document.getElementById('serialInput');
const sendBtn = document.getElementById('sendBtn');

function logToTerminal(text, type = 'normal') {
    const span = document.createElement('span');
    if (type === 'error') span.className = 'text-red-400';
    else if (type === 'system') span.className = 'text-slate-500';
    else span.className = 'text-cyan-400';
    
    span.textContent = text;
    terminal.appendChild(span);
    terminal.scrollTop = terminal.scrollHeight;
}

connectBtn.addEventListener('click', async () => {
    if (port) {
        await disconnectSerial();
        return;
    }

    try {
        port = await navigator.serial.requestPort();
        
        await port.open({ baudRate: 115200 });
        
        connectBtnText.textContent = "DISCONNECT SERIAL";
        connectStatusDot.classList.replace('bg-slate-500', 'bg-emerald-500');
        connectStatusDot.classList.add('shadow-[0_0_8px_rgba(16,185,129,0.8)]');
        serialInput.disabled = false;
        sendBtn.disabled = false;
        
        terminal.innerHTML = '';
        logToTerminal('Connected to ' + port.getInfo().usbProductId + '...\n\n', 'system');

        let decoder = new TextDecoderStream();
        inputDone = port.readable.pipeTo(decoder.writable);
        inputStream = decoder.readable;
        reader = inputStream.getReader();
        readLoop();

    } catch (err) {
        logToTerminal('\nError: ' + err.message + '\n', 'error');
    }
});

async function readLoop() {
    try {
        while (true) {
            const { value, done } = await reader.read();
            if (value) {
                logToTerminal(value);
            }
            if (done) {
                reader.releaseLock();
                break;
            }
        }
    } catch (error) {
    }
}

async function disconnectSerial() {
    if (reader) {
        await reader.cancel();
        await inputDone.catch(() => {});
        reader = null;
        inputDone = null;
    }
    if (outputStream) {
        await outputStream.getWriter().close();
        await outputDone;
        outputStream = null;
        outputDone = null;
    }
    await port.close();
    port = null;
    
    connectBtnText.textContent = "CONNECT SERIAL";
    connectStatusDot.classList.replace('bg-emerald-500', 'bg-slate-500');
    connectStatusDot.classList.remove('shadow-[0_0_8px_rgba(16,185,129,0.8)]');
    serialInput.disabled = true;
    sendBtn.disabled = true;
    logToTerminal('\nDisconnected.\n', 'system');
}

clearBtn.addEventListener('click', () => {
    terminal.innerHTML = '';
});

async function sendData() {
    if (!port || !serialInput.value) return;
    
    const encoder = new TextEncoder();
    const writer = port.writable.getWriter();
    await writer.write(encoder.encode(serialInput.value + '\n'));
    writer.releaseLock();
    
    logToTerminal('> ' + serialInput.value + '\n', 'system');
    serialInput.value = '';
}

sendBtn.addEventListener('click', sendData);
serialInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') sendData();
});
