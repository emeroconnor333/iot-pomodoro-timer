const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const express = require('express');
const app = express();

const PORT = '/dev/cu.usbmodem101';

let latestLog = '';

const port = new SerialPort({ path: PORT, baudRate: 9600 });
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

parser.on('data', (line) => {
  latestLog = line.trim();
  console.log(latestLog);
});

app.get('/', (req, res) => {
  res.send(`
    <!DOCTYPE html>
    <html>
    <head>
      <title>Pomodoro Timer</title>
      <style>
        body { font-family: monospace; 
               display: flex; justify-content: center; 
               align-items: center; height: 100vh; margin: 0;
               background: #c0392b; color: #fff; }
        #timer { font-size: 5rem; letter-spacing: 4px; }
        #label { font-size: 1rem; text-align: center; 
                 color: #98fb98; margin-top: 10px; }
      </style>
      <script>
        async function update() {
        const res = await fetch('/log');
        const text = await res.text();
        document.getElementById('timer').innerText = text;
        }
        setInterval(update, 500);
      </script>
    </head>
    <body>
      <div>
        <div id="timer">--:--</div>
        <div id="label">Minutes Left</div>
      </div>
    </body>
    </html>
  `);
});

app.get('/log', (req, res) => {
  res.send(latestLog);
});

app.listen(3000, () => {
  console.log('Open http://localhost:3000 in your browser');
});