// Authors: Grok and Eldon R. Brown - WA0UWH
// Rev: 20261104f
// Mock_Server - fakes the ESPHome v3 web_server (index, /0.js, /events SSE, POST actions)
// (basic auth guest:grest only when MOCK_AUTH=1) so www/erb_ui.js can be tested in a browser. Usage: node Mock_Server.js [port]
const http = require('http'), fs = require('fs'), path = require('path');
const Port = +process.argv[2] || 8099;
const JS = path.join(__dirname, '..', 'www', 'erb_ui.js');
const S = {
  'sensor/Free Heap': [23456, '23456 B'], 'sensor/Vdd': [3.29, '3.29 V'], 'sensor/WiFi Signal': [-61, '-61 dBm'],
  'sensor/Uptime': [3725, '3725 s'], 'sensor/Heap Max Block': [18000, '18000 B'], 'sensor/Loop Time': [16, '16 ms'],
  'text_sensor/ESPHome Version': [null, '2026.10.2'], 'text_sensor/Build Name': [null, 'ERB_EspWebServer_Esp8266_NodeMcu_20261104f'], 'text_sensor/Build Time': [null, '2026-11-04 17:55:00 -0800'], 'text_sensor/Reset Reason': [null, 'Power On'],
  'text_sensor/Device Info': [null, '2026.10.2|Flash: 4096kB Speed:40MHz Mode:DIO|Chip: 0x00E42F'],
  'text_sensor/IP Address': [null, '10.10.10.42'], 'text_sensor/Connected SSID': [null, 'MockNet'],
  'text_sensor/Connected BSSID': [null, 'AA:BB:CC:DD:EE:FF'], 'text_sensor/MAC Address': [null, '5C:CF:7F:12:34:56'],
  'text_sensor/WiFi Scan': [null, 'MockNet: -61dB\nNeighbor-5G: -78dB\nWA0UWH-Lab: -70dB'],
  'text_sensor/Clock': [null, ''], 'switch/Red LED': [true, 'ON'], 'switch/Green LED': [false, 'OFF'], 'switch/Blue LED': [false, 'OFF'], 'switch/Heartbeat': [true, 'ON'],
  'binary_sensor/Input 1': [false, 'OFF'], 'binary_sensor/Status': [true, 'ON'], 'button/Restart': [null, ''], 'button/Restart (Safe Mode)': [null, ''],
};
const Clients = new Set(), Posts = [];
const Auth = 'Basic ' + Buffer.from('guest:grest').toString('base64');   // same as the device
let Denied = 0;
function Msg(id) { const [v, s] = S[id]; return `event: state\ndata: ${JSON.stringify({ id, value: v, state: s })}\n\n`; }
function Push(id) { for (const c of Clients) c.write(Msg(id)); }
// Seed 30 min of heap history quickly is not possible over SSE; emit every 300 ms instead.
setInterval(() => {
  const h = 20000 + Math.round(4000 * Math.sin(Date.now() / 4000) + Math.random() * 800);
  S['sensor/Free Heap'] = [h, h + ' B']; Push('sensor/Free Heap');
  const u = S['sensor/Uptime'][0] + 1; S['sensor/Uptime'] = [u, u + ' s']; Push('sensor/Uptime');
  const d = new Date(); S['text_sensor/Clock'][1] = d.toISOString().slice(0, 19).replace('T', ' ') + ' UTC'; Push('text_sensor/Clock');
}, 300);
http.createServer((q, r) => {
  const u = decodeURIComponent(q.url.split('?')[0]);
  if (u === '/posts') { r.writeHead(200); return r.end(JSON.stringify({ Posts, Denied })); }
  if (process.env.MOCK_AUTH && q.headers.authorization !== Auth) {
    Denied++; r.writeHead(401, { 'WWW-Authenticate': 'Basic realm="Login Required"' }); return r.end('401');
  }
  if (u === '/update' && q.method === 'POST') {
    let n = 0; q.on('data', c => n += c.length); q.on('end', () => { Posts.push('/update ' + n + 'B'); r.writeHead(200); r.end('Update Successful!'); }); return;
  }
  if (u === '/') { r.writeHead(200, { 'Content-Type': 'text/html' }); return r.end('<!DOCTYPE html><html><head><meta charset=UTF-8><link rel=icon href=data:></head><body><script type=module src=/0.js></script><esp-app></esp-app></body></html>'); }
  if (u === '/0.js') { r.writeHead(200, { 'Content-Type': 'text/javascript' }); return r.end(fs.readFileSync(JS)); }
  
  if (u === '/events') {
    r.writeHead(200, { 'Content-Type': 'text/event-stream', 'Cache-Control': 'no-cache' });
    r.write(`event: ping\ndata: ${JSON.stringify({ title: 'ERB EspWebServer' })}\n\n`);
    Object.keys(S).forEach(k => r.write(Msg(k).replace('event: state', 'event: state_detail_all')));
    Clients.add(r); q.on('close', () => Clients.delete(r)); return;
  }
  if (q.method === 'POST') {
    Posts.push(u); const m = u.match(/^\/(switch\/.+)\/(toggle|turn_on|turn_off)$/);
    if (m && S[m[1]]) { const on = m[2] === 'toggle' ? !S[m[1]][0] : m[2] === 'turn_on'; S[m[1]] = [on, on ? 'ON' : 'OFF']; Push(m[1]); }
    r.writeHead(200); return r.end();
  }
  r.writeHead(404); r.end();
}).listen(Port, () => console.log('mock on ' + Port));
// End
