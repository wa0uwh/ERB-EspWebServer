// Authors: Grok and Eldon R. Brown - WA0UWH
// Rev: 20261104i
// Shoot_Tabs - headless screenshots of each ERB UI tab against Mock_Server.js.
const puppeteer = require('/workspace/repos/grok-voice-reader-extension/chrome-extension/node_modules/puppeteer-core');
const Out = require('path').join(__dirname, '..', 'screens') + (process.env.MOCK_NO_LEDS ? '/PlainLed48' : '');
require('fs').mkdirSync(Out, { recursive: true });
(async () => {
  const b = await puppeteer.launch({ executablePath: '/opt/google/chrome/chrome', headless: 'new', args: ['--no-sandbox'] });
  const p = await b.newPage(); await p.setViewport({ width: 1000, height: 820 });
  if (process.env.MOCK_AUTH) await p.authenticate({ username: 'guest', password: 'grest' });
  const Errs = []; p.on('pageerror', e => Errs.push(e.message)); p.on('dialog', d => d.accept());
  await p.goto('http://127.0.0.1:8099/'); await new Promise(r => setTimeout(r, 6000));
  await p.evaluate(() => { document.getElementById('hMin').value = '5'; document.getElementById('hMin').dispatchEvent(new Event('change', { bubbles: true })); });
  if (await p.$('[data-led="Green LED"]')) await p.click('[data-led="Green LED"]');
  console.log('LED BUTTONS', JSON.stringify(await p.$$eval('[data-led]', b => b.map(x => x.dataset.led)))); await new Promise(r => setTimeout(r, 800));
  await p.screenshot({ path: Out + '/1_Home.png', fullPage: true });
  let n = 2;
  for (const t of ['info', 'admin', 'help', 'all']) {
    await p.click(`[data-pg="${t}"]`); await new Promise(r => setTimeout(r, 700));
    await p.screenshot({ path: `${Out}/${n++}_${t[0].toUpperCase() + t.slice(1)}.png`, fullPage: true });
  }
  await p.click(`[data-pg="admin"]`); await p.click('[data-act="button/Restart/press"]');
  await p.click(`[data-pg="home"]`); await p.click('[data-led="Heartbeat"]'); await new Promise(r => setTimeout(r, 500));
  await p.click(`[data-pg="admin"]`);
  const Dlg = []; p.on('dialog', d => Dlg.push(d.message().split('\n')[0]));
  const Fw = '/tmp/ERB_EspWebServer_Esp32S3_SuperMini_PlainLed48_20261104i.ota.bin'; require('fs').writeFileSync(Fw, Buffer.alloc(4096, 7));
  await (await p.$('#otaFile')).uploadFile(Fw);
  console.log('CHOSEN', await p.$eval('#otaName', e => e.textContent));
  await p.click('#otaF button'); await new Promise(r => setTimeout(r, 1000));
  console.log('OTA MSG', await p.$eval('#otaMsg', e => e.textContent), 'DIALOGS', JSON.stringify(Dlg));
  await p.click('[data-pg="admin"]'); await p.screenshot({ path: Out + '/3_Admin.png', fullPage: true });
  const nr = await fetch('http://127.0.0.1:8099/events'); console.log('NO-AUTH /events ->', nr.status);
  const posts = await (await fetch('http://127.0.0.1:8099/posts')).json();
  console.log('POSTS', JSON.stringify(posts), 'ERRORS', JSON.stringify(Errs));
  await b.close(); process.exit(0);
})();
// End
