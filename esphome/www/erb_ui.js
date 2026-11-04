// Authors: Grok and Eldon R. Brown - WA0UWH
// Rev: 20261104d
// ERB_UI - custom multi-page UI for the standalone ESPHome ERB-EspWebServer.
// Recreates the 2015 Home / Help / Admin / Info pages + Navigator, from flash, no internet.
// Data: /events SSE ("state" events, id = "<domain>/<Entity Name>"); actions: REST POST.
(function () {
  var Hist_Min = 30, Hist = [], S = {}, Page = 'home', Hits = 0;
  var Rev = '20261104d';
  var css = 'body{background:#ddddee;font-family:Times New Roman,serif;margin:8px;color:#000}' +
    'h1{text-align:center;margin:4px}h1 small{font-size:13px;font-weight:normal;display:block}' +
    '.Nav{width:100%;border-collapse:collapse}.Nav td{padding:2px}.Nav a{color:#00e;cursor:pointer;text-decoration:underline}' +
    '.Nav a.On{font-weight:bold;color:#551a8b}.Ctr{text-align:center}.Box{display:flex;flex-wrap:wrap;gap:14px;justify-content:center;margin:10px 0}' +
    'pre{font-size:14px;background:#eef;border:1px solid #99a;padding:8px;overflow:auto}' +
    'button{font-size:15px;margin:3px;padding:3px 12px}.Led{min-width:110px}.LedOn{font-weight:bold}' +
    'table.Ent{border-collapse:collapse;margin:auto}table.Ent td{border:1px solid #99a;padding:2px 8px}#Conn{font-size:12px}';
  var st = document.createElement('style'); st.textContent = css; document.head.appendChild(st);

  function $(i) { return document.getElementById(i); }
  function V(n) { var e = S[n]; return e ? e.state : '--'; }
  function N(n) { var e = S[n]; return e && typeof e.value === 'number' ? e.value : NaN; }
  function Esc(t) { return String(t).replace(/[&<>"]/g, function (c) { return { '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]; }); }
  function Post(url) { return fetch(url, { method: 'POST' }); }
  function Enc(n) { return n.split('/').map(encodeURIComponent).join('/'); }
  function Key(k) { return k.replace(/\W/g, '_'); }

  // ---- SVG pieces in the original's style (radial-gradient faces, black rims) ----
  var Grad = '<defs><radialGradient id="gFace"><stop offset="0%" stop-color="#DDDDDD"/><stop offset="100%" stop-color="#111122"/></radialGradient>' +
    '<radialGradient id="gHub"><stop offset="0%" stop-color="Red"/><stop offset="100%" stop-color="Black"/></radialGradient></defs>';
  function ClockSvg() {
    var t = '';
    for (var i = 0; i < 12; i++) t += '<line y1="-62" y2="-54" stroke="white" stroke-width="3" transform="rotate(' + i * 30 + ')"/>';
    return '<svg width="150" height="150" viewBox="-75 -75 150 150">' + Grad +
      '<circle r="72" fill="url(#gFace)" stroke="black" stroke-width="3"/>' + t +
      '<line id="cH" y2="-34" stroke="white" stroke-width="6" stroke-linecap="round"/>' +
      '<line id="cM" y2="-52" stroke="white" stroke-width="4" stroke-linecap="round"/>' +
      '<line id="cS" y2="-58" stroke="#FF4400" stroke-width="1.5"/><circle r="5" fill="url(#gHub)"/>' +
      '<text id="cD" y="30" text-anchor="middle" fill="#44FF00" font-size="10">--</text></svg>';
  }
  function InstSvg(id, label, lo, hi) {
    var t = '';
    for (var i = 0; i <= 10; i++) t += '<line y1="-62" y2="-54" stroke="white" stroke-width="2" transform="rotate(' + (-135 + i * 27) + ')"/>';
    return '<svg width="150" height="150" viewBox="-75 -75 150 150" data-lo="' + lo + '" data-hi="' + hi + '" id="' + id + '">' + Grad +
      '<circle r="72" fill="url(#gFace)" stroke="black" stroke-width="3"/>' + t +
      '<text y="-22" text-anchor="middle" fill="white" font-size="12">' + label + '</text>' +
      '<text x="-40" y="52" fill="white" font-size="9">' + lo + '</text><text x="40" y="52" fill="white" font-size="9" text-anchor="end">' + hi + '</text>' +
      '<line class="Ndl" y2="-56" stroke="red" stroke-width="2.5" transform="rotate(-135)"/><circle r="5" fill="url(#gHub)"/>' +
      '<text class="Val" y="40" text-anchor="middle" fill="#44FF00" font-size="13">--</text></svg>';
  }
  function SetInst(id, v, txt) {
    var s = $(id); if (!s) return;
    var lo = +s.dataset.lo, hi = +s.dataset.hi, f = isNaN(v) ? 0 : Math.max(0, Math.min(1, (v - lo) / (hi - lo)));
    s.querySelector('.Ndl').setAttribute('transform', 'rotate(' + (-135 + 270 * f) + ')');
    s.querySelector('.Val').textContent = txt;
  }
  function LineSvg() {   // Free Heap history, like B_SVG_Line_FH: lightblue field, white grid, red trace
    var W = 400, H = 150, g = '';
    for (var y = 15; y < H; y += 30) g += '<path d="M 15 ' + y + ' H ' + (W - 5) + '"/>';
    for (var x = 15; x < W; x += 65) g += '<path d="M ' + x + ' 5 V ' + (H - 5) + '"/>';
    return '<svg width="' + W + '" height="' + H + '" viewBox="0 0 ' + W + ' ' + H + '"><rect width="' + W + '" height="' + H + '" style="fill:lightblue;stroke:black;stroke-width:1"/>' +
      '<g style="fill:none;stroke:white;stroke-width:1">' + g + '</g><polyline id="fhL" style="fill:none;stroke:red;stroke-width:2" points=""/>' +
      '<text id="fhT" x="20" y="14" font-size="11">Free Heap</text></svg>';
  }
  function DrawLine() {
    var p = $('fhL'); if (!p || !Hist.length) return;
    var now = Date.now(), span = Hist_Min * 60000, lo = Infinity, hi = -Infinity;
    var h = Hist.filter(function (x) { return x[0] >= now - span; }); if (!h.length) return;
    h.forEach(function (x) { lo = Math.min(lo, x[1]); hi = Math.max(hi, x[1]); });
    if (hi - lo < 1000) { lo -= 500; hi += 500; }
    p.setAttribute('points', h.map(function (x) {
      return (15 + 380 * (1 - (now - x[0]) / span)).toFixed(1) + ',' + (140 - 125 * (x[1] - lo) / (hi - lo)).toFixed(1);
    }).join(' '));
    $('fhT').textContent = 'Free Heap, last ' + Hist_Min + ' min: ' + (lo / 1000).toFixed(1) + ' - ' + (hi / 1000).toFixed(1) + ' kB';
  }

  // ---- Pages ----
  function Row(k, n) { return k + ': <b id="v_' + Key(n) + '">' + Esc(V(n)) + '</b>\n'; }
  function IsS3() { return !!S['sensor/Chip Temp']; }
  var Pages = {
    home: function () {
      return '<div class="Ctr"><b>Dashboard</b></div><div class="Box">' + ClockSvg() + InstSvg('gHeap', 'Free Heap kB', 0, IsS3() ? 300 : 60) +
        (IsS3() ? InstSvg('gVdd', 'Chip Temp &#176;C', 20, 100) : InstSvg('gVdd', 'Vdd Volts', 2.5, 3.6)) +
        '</div><div class="Box">' + LineSvg() + '</div><div class="Ctr">History: <select id="hMin">' +
        [5, 15, 30, 60].map(function (m) { return '<option' + (m == Hist_Min ? ' selected' : '') + '>' + m + '</option>'; }).join('') + '</select> min</div>' +
        '<div class="Ctr" style="margin-top:10px"><b>LED Control</b><br>' +
        ['Red', 'Green', 'Blue'].map(function (c) { return '<button class="Led" data-led="' + c + ' LED">' + c + ' LED is <span>' + Esc(V('switch/' + c + ' LED')) + '</span></button>'; }).join('') +
        '<button class="Led" data-led="Heartbeat">Heartbeat is <span>' + Esc(V('switch/Heartbeat')) + '</span></button>' +
        '<br>Input 1 is <b id="v_binary_sensor_Input_1">' + Esc(V('binary_sensor/Input 1')) + '</b></div>';
    },
    info: function () {
      return '<pre>' + Row('Version', 'text_sensor/ESPHome Version') + Row('UI Rev', 'x/rev') + Row('Uptime', 'sensor/Uptime') +
        Row('Reset Reason', 'text_sensor/Reset Reason') + Row('Device', 'text_sensor/Device Info') +
        Row('IP Address', 'text_sensor/IP Address') + Row('SSID', 'text_sensor/Connected SSID') + Row('BSSID', 'text_sensor/Connected BSSID') +
        Row('RSSI', 'sensor/WiFi Signal') + Row('MAC', 'text_sensor/MAC Address') + Row('Free Heap', 'sensor/Free Heap') +
        Row('Heap Max Block', 'sensor/Heap Max Block') + Row('Loop Time', 'sensor/Loop Time') +
        (S['sensor/Vdd'] ? Row('Vdd', 'sensor/Vdd') : '') + (IsS3() ? Row('Chip Temp', 'sensor/Chip Temp') : '') +
        (S['sensor/PSRAM Free'] ? Row('PSRAM Free', 'sensor/PSRAM Free') : '') +
        '\nCopyright (c) 2015, Eldon R. Brown - ERB - WA0UWH\nESPHome port: Grok and Eldon R. Brown - WA0UWH</pre>';
    },
    admin: function () {
      return '<div class="Ctr"><b>Admin</b><br><button data-act="button/Restart/press" data-q="Reboot the device?">Reboot</button>' +
        '<button data-act="button/Restart (Safe Mode)/press" data-q="Reboot into Safe Mode (OTA only)?">Safe Mode</button></div>' +
        '<pre><b>WiFi Scan</b>\n<span id="v_text_sensor_WiFi_Scan">' + Esc(V('text_sensor/WiFi Scan')) + '</span></pre>' +
        '<form id="otaF"><b>Firmware update</b> (.ota.bin): <input type="file" id="otaFile" accept=".bin"> <button type="submit">Upload</button> <span id="otaMsg"></span></form>' +
        '<p>To change networks: if the device cannot join its WiFi it starts hotspot <b>ERB-EspWebServer-Setup</b>. Join it and a captive portal page lets you pick a new network.</p>';
    },
    help: function () {
      return '<pre><b>Pages:</b>\n  Home  = Dashboard: clock, Free Heap and Vdd/Temp meters, heap history, LED control\n' +
        '  Help  = This Message\n  Admin = Reboot, Safe Mode, WiFi scan\n  Info  = General Information\n  All   = Every ESPHome entity (raw list)\n\n' +
        '<b>REST (like the old /t/redled, /q/... links):</b>\n  POST /switch/Red LED/toggle     = Toggle Red Led\n  POST /switch/Red LED/turn_on    = Turn Red Led ON\n' +
        '  POST /switch/Red LED/turn_off   = Turn Red Led OFF\n  GET  /sensor/Free Heap          = Free Heap (JSON)\n  GET  /text_sensor/IP Address    = IP (JSON)\n' +
        '  GET  /events                    = Live event stream\n\nAll pages update live; no refresh or slider needed.\n' +
        'mDNS name: <b>' + Esc(location.hostname) + '</b></pre>';
    },
    all: function () {
      return '<table class="Ent"><tr><td><b>Entity</b></td><td><b>State</b></td><td></td></tr>' + Object.keys(S).filter(function (k) { return k.indexOf('x/'); }).sort().map(function (k) {
        var d = k.split('/')[0], act = d == 'switch' ? '<button data-act="' + Esc(k) + '/toggle">toggle</button>' : d == 'button' ? '<button data-act="' + Esc(k) + '/press" data-q="Press ' + Esc(k) + '?">press</button>' : '';
        return '<tr><td>' + Esc(k) + '</td><td id="a_' + Key(k) + '">' + Esc(S[k].state) + '</td><td>' + act + '</td></tr>';
      }).join('') + '</table>';
    }
  };
  function Nav() {
    return '<table class="Nav"><tr><td align="left" width="40%">' + ['home', 'help', 'admin', 'info', 'all'].map(function (p) {
      return ' | <a data-pg="' + p + '"' + (p == Page ? ' class="On"' : '') + '>' + (p == 'all' ? 'All' : p[0].toUpperCase() + p.slice(1)) + '</a>';
    }).join('') + ' |</td><td align="center" id="navClk">--</td><td align="right" width="40%">' +
      ' | <a href="https://esphome.io/components/web_server/" target="Docs">ESPHome</a> | <a href="https://github.com/wa0uwh/ERB-EspWebServer" target="GitHub">GitHub</a> |</td></tr></table>' +
      '<div class="Ctr" id="navStat">--</div>';
  }
  function Show(p) {
    Page = p; Hits++;
    $('erbNav').innerHTML = Nav(); $('erbPage').innerHTML = Pages[p](); Tick(); Refresh();
  }
  function Refresh() {
    var fh = N('sensor/Free Heap') / 1000;
    $('navStat').textContent = (isNaN(fh) ? '--' : fh.toFixed(3)) + ' - ' + Hits + ' - ' + Rev;
    SetInst('gHeap', fh, isNaN(fh) ? '--' : fh.toFixed(1));
    var t = IsS3() ? 'sensor/Chip Temp' : 'sensor/Vdd'; SetInst('gVdd', N(t), V(t));
    Object.keys(S).forEach(function (k) {
      var e = $('v_' + Key(k)) || $('a_' + Key(k)); if (e) e.textContent = S[k].state;
    });
    document.querySelectorAll('[data-led]').forEach(function (b) {
      var s = V('switch/' + b.dataset.led); b.querySelector('span').textContent = s; b.classList.toggle('LedOn', s == 'ON');
      b.style.background = s == 'ON' ? { 'Red LED': '#f66', 'Green LED': '#6d6', 'Blue LED': '#69f', 'Heartbeat': '#f9c' }[b.dataset.led] : '';
    });
    DrawLine();
  }
  // Pacific time, PST/PDT label, independent of the viewing computer's zone
  var Tz = 'America/Los_Angeles';
  function Pac(d) {
    var o = {}; new Intl.DateTimeFormat('en-US', { timeZone: Tz, hour12: false, year: 'numeric', month: '2-digit', day: '2-digit',
      hour: '2-digit', minute: '2-digit', second: '2-digit', timeZoneName: 'short' }).formatToParts(d).forEach(function (p) { o[p.type] = p.value; });
    o.hour = o.hour == '24' ? '00' : o.hour; return o;
  }
  function Tick() {
    var p = Pac(new Date()), h = +p.hour, m = +p.minute, sec = +p.second;
    var txt = p.year + '-' + p.month + '-' + p.day + ' ' + p.hour + ':' + p.minute + ':' + p.second + ' ' + p.timeZoneName;
    if ($('navClk')) $('navClk').textContent = txt;
    function R(i, a) { var e = $(i); if (e) e.setAttribute('transform', 'rotate(' + a + ')'); }
    R('cH', (h % 12) * 30 + m / 2); R('cM', m * 6 + sec / 10); R('cS', sec * 6);
    if ($('cD')) $('cD').textContent = p.hour + ':' + p.minute + ':' + p.second + ' ' + p.timeZoneName;
  }

  // ---- Wiring ----
  document.addEventListener('click', function (ev) {
    var t = ev.target.closest('[data-pg],[data-led],[data-act]'); if (!t) return;
    if (t.dataset.pg) return Show(t.dataset.pg);
    if (t.dataset.led) return Post('/switch/' + Enc(t.dataset.led) + '/toggle');
    if (t.dataset.q && !confirm(t.dataset.q)) return;
    Post('/' + Enc(t.dataset.act));
  });
  document.addEventListener('submit', function (ev) {
    if (ev.target.id != 'otaF') return; ev.preventDefault();
    var f = $('otaFile').files[0]; if (!f || !confirm('Upload ' + f.name + ' and reboot?')) return;
    var fd = new FormData(); fd.append('update', f, f.name); $('otaMsg').textContent = 'uploading...';
    fetch('/update', { method: 'POST', body: fd }).then(function (r) { return r.text().then(function (t) {
      $('otaMsg').textContent = r.ok ? 'done, rebooting: ' + t : 'failed (' + r.status + ') - this board may not have web OTA'; }); })
      .catch(function () { $('otaMsg').textContent = 'upload failed'; });
  });
  document.addEventListener('change', function (ev) { if (ev.target.id == 'hMin') { Hist_Min = +ev.target.value; DrawLine(); } });

  function OnState(ev) {
    var d; try { d = JSON.parse(ev.data); } catch (e) { return; }
    var k = d.name_id || d.id; if (!k) return;
    var isNew = !S[k]; S[k] = d;
    if (k == 'sensor/Free Heap' && typeof d.value == 'number') {
      Hist.push([Date.now(), d.value]);
      while (Hist.length && Hist[0][0] < Date.now() - 60 * 60000) Hist.shift();
    }
    if (isNew && (Page == 'all' || Page == 'info' || (Page == 'home' && k == 'sensor/Chip Temp'))) Show(Page); else Refresh();
  }
  function Connect() {
    var es = new EventSource('/events');
    es.addEventListener('state', OnState);
    es.addEventListener('state_detail_all', OnState);
    es.addEventListener('ping', function (ev) {
      $('Conn').textContent = 'live';
      try { var c = JSON.parse(ev.data); if (c.title) { document.title = c.title; $('erbTitle').firstChild.textContent = c.title; } } catch (e) {}
    });
    es.onerror = function () { $('Conn').textContent = 'reconnecting...'; };
  }
  function Start() {
    document.body.innerHTML = '<h1 id="erbTitle">ERB EspWebServer<small>ERB\'s Esp Experimental Web Server (ESPHome) - <span id="Conn">connecting</span></small></h1>' +
      '<div id="erbNav"></div><hr><div id="erbPage"></div><hr><div class="Ctr" style="font-size:12px">WA0UWH - ERB-EspWebServer - Rev ' + Rev + '</div>';
    S['x/rev'] = { state: Rev };
    Show('home'); Connect(); setInterval(Tick, 1000); setInterval(DrawLine, 10000);
  }
  if (document.readyState == 'loading') document.addEventListener('DOMContentLoaded', Start); else Start();
})();
// End
