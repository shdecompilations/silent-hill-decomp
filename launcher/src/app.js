const $ = s => document.querySelector(s);
const $$ = s => [...document.querySelectorAll(s)];

// tabs
$$('.nav-btn').forEach(btn=>{
  btn.addEventListener('click', ()=>{
    $$('.nav-btn').forEach(b=>b.classList.remove('active'));
    btn.classList.add('active');
    const tab = btn.dataset.tab;
    $$('.tab').forEach(t=>t.classList.remove('active'));
    $(`#tab-${tab}`).classList.add('active');
  });
});

function log(msg){
  const el = $('#log-output');
  const atBottom = el.scrollTop + el.clientHeight >= el.scrollHeight - 10;
  el.textContent += `\n> ${msg}`;
  if(atBottom) el.scrollTop = el.scrollHeight;
  // also mirror to asset log if visible
  const al = $('#asset-log');
  if(al) al.textContent += `\n${msg}`;
}

function fakeProgress(done){
  const bar = $('#asset-bar'), pct = $('#asset-pct'), prog = $('#asset-progress');
  if(!bar) return;
  prog.style.display='block';
  let v=0;
  const t = setInterval(()=>{
    v += Math.random()*14 + 4;
    if(v>=100){ v=100; clearInterval(t); done&&done(); }
    bar.style.width = v+'%';
    pct.textContent = Math.floor(v)+'%';
    if(Math.floor(v)%13===0) log(`extract: ${Math.floor(v)}% — ${['SILENT.','HILL.','1ST/BODYPROG.BIN','VIN/MAP0_S00.BIN'][Math.floor(Math.random()*4)]}`);
  },120);
}

// launch
$('#btn-launch')?.addEventListener('click', ()=>{
  const v = $('#sel-version').value, entry = $('#sel-entry').value;
  log(`launch: version=${v} entry=${entry} renderer=${$('#set-renderer')?.value||'gl'}`);
  log(`cmd: ./build-pc/silenthill --assets assets/${v} --rom rom/${v} --entry ${entry}`);
  const btn = $('#btn-launch');
  btn.disabled=true; btn.innerHTML='<span class="btn-icon">◍</span> STARTING…';
  setTimeout(()=>{
    log('gpu: frame 1 — PutDispEnv 320×240');
    log('fs: async queue drained (Fs_QueueChunksLoad=true)');
    log('game: GameState_Init → KonamiLogo (Screen_Init ok)');
    btn.disabled=false; btn.innerHTML='<span class="btn-icon">▶</span> LAUNCH <span class="btn-sub">— running</span>';
  },900);
});

$('#btn-verify')?.addEventListener('click', ()=>{
  log('verify: sha256sum --check configs/USA/checksum.sha');
  setTimeout(()=>log('verify: 46/46 ok — PSX binary matches retail (SLUS_007.07)'),700);
  setTimeout(()=>log('verify: data_audit — maps 66.1% (133 raw), bodyprog 90%'),1200);
});

$('#btn-rebuild')?.addEventListener('click', ()=>{
  log('build: make GAME_VERSION=USA generate');
  log('build: make GAME_VERSION=USA CHECKSUM=1 build -j8');
  let n=0; const iv=setInterval(()=>{
    log(['cc  src/bodyprog/sys/game_main.c','cc  src/maps/map0_s00/header.c','ld  build/USA/out/VIN/MAP0_S00.BIN.elf','ld  build/USA/out/1ST/BODYPROG.BIN.elf'][n%4]+' — ok');
    if(++n>8){ clearInterval(iv); log('build: 46/46 MATCH — done in 14.2s'); }
  },320);
});

$('#btn-extract')?.addEventListener('click', ()=>{
  log('extract: dumpsxiso -x rom/USA -s rom/USA/layout.xml image/SLUS-00707.bin');
  fakeProgress(()=>{ log('extract: silentassets → assets/USA/ — 2074 files'); log('extract: filetable.c.USA.inc + fileenum.h generated'); });
});
$('#asset-drop')?.addEventListener('click', ()=> $('#file-input')?.click());
$('#file-input')?.addEventListener('change', e=>{
  const f=e.target.files[0]; if(!f) return;
  log(`drop: ${f.name} (${(f.size/1e6).toFixed(1)} MB) — parsing…`);
  fakeProgress(()=> log(`drop: verified — ${f.name} ready for extraction`));
});
$('#set-scale')?.addEventListener('change', e=>{
  const v=e.target.value; const m=$('#meta-res'); if(m) m.textContent=v.split('—')[0].trim()||v;
});
$('#set-renderer')?.addEventListener('change', e=>{
  const m=$('#meta-renderer'); if(m) m.textContent = e.target.value==='bgfx' ? 'bgfx (Vulkan)' : 'OpenGL 3.3';
});
$('#btn-clear-log')?.addEventListener('click',()=>{ $('#log-output').textContent='> log cleared'; });

// tools
$$('[data-tool]').forEach(b=>{
  b.addEventListener('click',()=>{
    const tool=b.dataset.tool, out=$('#tool-log');
    const msgs={
      audit:'python3 tools/data_audit.py --detail\nmain 100%  bodyprog 90%  maps 66.1%  screens 91.7% — 133 raw left',
      naming:'python3 tools/naming_progress.py build/USA/out/1ST/BODYPROG.BIN.elf\nfuncs: 312 named, 94 func_/sharedFunc_ (23% auto) — see naming_reports/',
      check:'make GAME_VERSION=USA CHECKSUM=1 build\n46/46 MATCH — SLUS_007.07, BODYPROG.BIN, 43 maps ok',
      'make-shiftable':'python3 pc/tools/make_shiftable.py --check\nsrc/main/main.c: g_OvlBodyprog 0x80024B60 → sh_overlay_base()\nsrc/maps/*: 43 hardcodes — run --fix to patch',
    };
    out.textContent = `› ${msgs[tool]||tool}`;
    log(`tool: ${tool} — done`);
  });
});

// drag
const drop=$('#asset-drop');
if(drop){
  drop.addEventListener('dragover',e=>{ e.preventDefault(); drop.style.borderColor='var(--accent)'; });
  drop.addEventListener('dragleave',()=> drop.style.borderColor='');
  drop.addEventListener('drop',e=>{
    e.preventDefault(); drop.style.borderColor='';
    const f=e.dataTransfer.files[0]; if(!f) return;
    log(`drop: ${f.name} — queued`); fakeProgress(()=> log('drop: extraction ready'));
  });
}

// easter: konami
let konami=[]; const seq=['ArrowUp','ArrowUp','ArrowDown','ArrowDown','ArrowLeft','ArrowRight','ArrowLeft','ArrowRight','b','a'];
window.addEventListener('keydown',e=>{
  konami.push(e.key); konami=konami.slice(-10);
  if(konami.join(',')===seq.join(',')){ log('egg: ↑↑↓↓←→←→BA — static burst enabled'); document.body.style.filter='contrast(1.15) brightness(0.95)'; setTimeout(()=>document.body.style.filter='',1200); }
});
console.log('Silent Hill launcher ready');
