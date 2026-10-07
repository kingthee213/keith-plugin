# Builds design/editor-prototype.html with embedded OFL fonts. Authoring only; runtime is offline.
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$fontDir = Join-Path $root "_fonttmp"
$out = Join-Path $root "editor-prototype.html"

$bangers = Get-Content (Join-Path $fontDir "bangers.b64") -Raw
$monoReg = Get-Content (Join-Path $fontDir "monoReg.b64") -Raw
$monoBold = Get-Content (Join-Path $fontDir "monoBold.b64") -Raw

$html = @'
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Keith  -  Editor Prototype</title>
<style>
@font-face {
  font-family: "Bangers";
  src: url(data:font/ttf;base64,%%BANGERS%%) format("truetype");
  font-weight: 400;
  font-style: normal;
  font-display: block;
}
@font-face {
  font-family: "Space Mono";
  src: url(data:font/ttf;base64,%%MONOREG%%) format("truetype");
  font-weight: 400;
  font-style: normal;
  font-display: block;
}
@font-face {
  font-family: "Space Mono";
  src: url(data:font/ttf;base64,%%MONOBOLD%%) format("truetype");
  font-weight: 700;
  font-style: normal;
  font-display: block;
}

:root {
  --ink: #0B0B12;
  --panel: #14141F;
  --panel-alt: #1C1C2A;
  --line: #0A0A0A;
  --paper: #F4EFE4;
  --green: #39FF14;
  --magenta: #FF2BD6;
  --gold: #FFC857;
  --cyan: #2EE6FF;
  --mute: #FF3B4A;
  --fill-a: #2A1A4A;
  --fill-b: #0F2A22;
  --fill-c: #2A1020;
  --fill-d: #1A1A10;
  --shadow: 4px 4px 0 var(--line);
}

* { box-sizing: border-box; margin: 0; padding: 0; }
html, body {
  height: 100%;
  background: #050508;
  color: var(--paper);
  font-family: "Space Mono", monospace;
  overflow: auto;
}
body {
  min-width: 960px;
  min-height: 640px;
}

/* Halftone atmosphere  -  original CSS, no external assets */
body::before {
  content: "";
  position: fixed;
  inset: 0;
  pointer-events: none;
  opacity: 0.12;
  background-image: radial-gradient(circle, #fff 1px, transparent 1.2px);
  background-size: 6px 6px;
  z-index: 0;
}

#app {
  position: relative;
  z-index: 1;
  width: 100%;
  height: 100%;
  min-height: 640px;
  padding: 10px;
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.comic {
  background: var(--panel);
  border: 5px solid var(--line);
  box-shadow: var(--shadow);
  position: relative;
}
.comic::after {
  content: "";
  position: absolute;
  inset: 0;
  pointer-events: none;
  box-shadow: inset 0 0 0 2px rgba(244,239,228,0.06);
}

.titlebar {
  display: flex;
  align-items: center;
  gap: 8px;
  height: 28px;
  padding: 0 10px;
  border-bottom: 4px solid var(--line);
  font-family: "Bangers", sans-serif;
  font-size: 22px;
  letter-spacing: 0.04em;
  text-transform: uppercase;
  color: var(--ink);
}
.titlebar.green { background: var(--green); }
.titlebar.magenta { background: var(--magenta); color: var(--paper); }
.titlebar.gold { background: var(--gold); }
.titlebar.paper { background: var(--paper); }

.star {
  width: 18px; height: 18px; flex: 0 0 18px;
}
.star polygon { fill: var(--gold); stroke: var(--line); stroke-width: 2; }

/* Top preset strip */
#preset {
  flex: 0 0 auto;
  display: grid;
  grid-template-columns: 200px 1fr;
  background: var(--fill-d);
  min-height: 64px;
}
#preset .brand {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 8px 14px;
  border-right: 5px solid var(--line);
  background: var(--ink);
}
#preset .brand h1 {
  font-family: "Bangers", sans-serif;
  font-size: 42px;
  color: var(--gold);
  -webkit-text-stroke: 2px var(--line);
  paint-order: stroke fill;
  line-height: 1;
}
#preset .brand .sub {
  font-size: 10px;
  color: var(--paper);
  opacity: 0.7;
  margin-top: 2px;
}
#preset .strip-body {
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  gap: 10px;
  padding: 8px 12px;
}
.family-chips { display: flex; gap: 6px; flex-wrap: wrap; }
.chip {
  font-family: "Bangers", sans-serif;
  font-size: 14px;
  padding: 4px 10px;
  border: 3px solid var(--line);
  background: var(--panel-alt);
  color: var(--paper);
  cursor: pointer;
  box-shadow: 2px 2px 0 var(--line);
}
.chip.active { background: var(--gold); color: var(--ink); }
.lists {
  display: flex; gap: 8px; flex: 1; min-width: 280px;
}
.listbox {
  flex: 1;
  min-width: 120px;
  max-height: 72px;
  overflow: auto;
  border: 3px solid var(--line);
  background: var(--panel);
}
.listbox .row {
  height: 24px;
  padding: 0 8px;
  font-size: 11px;
  display: flex; align-items: center;
  border-bottom: 1px solid #2a2a3a;
  cursor: pointer;
}
.listbox .row:nth-child(even) { background: var(--panel-alt); }
.listbox .row.sel {
  background: var(--fill-c);
  box-shadow: inset 6px 0 0 var(--magenta);
  color: var(--paper);
  font-weight: 700;
}
.btn-row { display: flex; gap: 6px; }
.btn {
  font-family: "Bangers", sans-serif;
  font-size: 14px;
  padding: 6px 12px;
  border: 3px solid var(--line);
  background: var(--green);
  color: var(--ink);
  cursor: pointer;
  box-shadow: 2px 2px 0 var(--line);
}
.btn.mag { background: var(--magenta); color: var(--paper); }
.btn.ghost { background: var(--panel-alt); color: var(--paper); }
.btn:active { transform: translate(1px,1px); box-shadow: 1px 1px 0 var(--line); }

/* Main three columns */
#main {
  flex: 1;
  display: grid;
  grid-template-columns: minmax(280px, 360px) minmax(360px, 1fr) minmax(260px, 320px);
  gap: 10px;
  min-height: 0;
}
@media (max-width: 1100px) {
  #main { grid-template-columns: minmax(240px, 1fr) minmax(300px, 1.2fr) minmax(220px, 0.9fr); }
}

.zone { display: flex; flex-direction: column; min-height: 0; overflow: hidden; }
.zone .body { flex: 1; padding: 10px; overflow: auto; min-height: 0; }

#drum { background: var(--fill-a); }
#synth { background: var(--fill-b); }
#fx { background: var(--fill-c); }

.pad-grid {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 8px;
  margin-bottom: 12px;
}
.pad {
  aspect-ratio: 1;
  min-height: 48px;
  border: 4px solid var(--line);
  background: var(--panel-alt);
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  cursor: pointer;
  user-select: none;
  position: relative;
  box-shadow: 3px 3px 0 var(--line);
}
.pad .name {
  font-family: "Bangers", sans-serif;
  font-size: 13px;
  color: var(--paper);
  text-align: center;
  line-height: 1.1;
}
.pad .note {
  font-size: 9px;
  color: var(--cyan);
  margin-top: 2px;
}
.pad.selected { outline: 3px solid var(--gold); outline-offset: 2px; }
.pad.hit { background: var(--green); }
.pad.hit .name { color: var(--ink); }
.pad.muted::before {
  content: "";
  position: absolute; inset: 6px;
  background: repeating-linear-gradient(-45deg, transparent, transparent 4px, var(--mute) 4px, var(--mute) 6px);
  opacity: 0.55;
  pointer-events: none;
}

.knob-row {
  display: flex;
  flex-wrap: wrap;
  gap: 10px 12px;
  margin-top: 6px;
}
.knob-wrap {
  width: 64px;
  text-align: center;
}
.knob {
  width: 44px; height: 44px;
  margin: 0 auto;
  border-radius: 50%;
  border: 3px solid var(--line);
  background: var(--panel-alt);
  position: relative;
  cursor: ns-resize;
  box-shadow: 2px 2px 0 var(--line);
}
.knob.sm { width: 36px; height: 36px; }
.knob .tick {
  position: absolute;
  left: 50%; top: 50%;
  width: 3px; height: 16px;
  background: var(--green);
  border: 1px solid var(--line);
  transform-origin: 50% 100%;
  transform: translate(-50%, -100%) rotate(0deg);
  border-radius: 1px;
}
.knob.mag .tick { background: var(--magenta); }
.knob-wrap label {
  display: block;
  margin-top: 4px;
  font-size: 10px;
  text-transform: uppercase;
  letter-spacing: 0.04em;
  color: var(--paper);
}
.knob-wrap .val {
  font-size: 10px;
  font-weight: 700;
  color: var(--gold);
}

.switch {
  width: 48px; height: 22px;
  border: 3px solid var(--line);
  border-radius: 11px;
  background: var(--panel-alt);
  position: relative;
  cursor: pointer;
  box-shadow: 2px 2px 0 var(--line);
  display: inline-block;
  vertical-align: middle;
}
.switch.on { background: var(--green); }
.switch.on.mag { background: var(--magenta); }
.switch .thumb {
  position: absolute;
  top: 1px; left: 2px;
  width: 14px; height: 14px;
  background: var(--paper);
  border: 2px solid var(--line);
  border-radius: 50%;
  transition: left 80ms linear;
}
.switch.on .thumb { left: 26px; }

.block {
  border: 3px solid var(--line);
  background: rgba(11,11,18,0.45);
  padding: 8px;
  margin-bottom: 8px;
}
.block h3 {
  font-family: "Bangers", sans-serif;
  font-size: 16px;
  color: var(--gold);
  margin-bottom: 6px;
  letter-spacing: 0.03em;
}
.engine-row {
  display: flex; align-items: center; gap: 10px; margin-bottom: 8px; flex-wrap: wrap;
}
.engine-row span.lbl { font-size: 11px; text-transform: uppercase; }
.seg {
  display: flex; border: 3px solid var(--line); box-shadow: 2px 2px 0 var(--line);
}
.seg button {
  font-family: "Bangers", sans-serif;
  font-size: 14px;
  padding: 4px 12px;
  border: none;
  border-right: 3px solid var(--line);
  background: var(--panel-alt);
  color: var(--paper);
  cursor: pointer;
}
.seg button:last-child { border-right: none; }
.seg button.active { background: var(--green); color: var(--ink); }

.meta {
  font-size: 10px;
  opacity: 0.65;
  margin-top: 8px;
}
.size-readout {
  position: fixed;
  right: 14px; bottom: 10px;
  z-index: 5;
  font-size: 10px;
  background: var(--paper);
  color: var(--ink);
  border: 3px solid var(--line);
  padding: 2px 8px;
  box-shadow: 2px 2px 0 var(--line);
}
</style>
</head>
<body>
<div id="app">

  <!-- PRESET -->
  <section id="preset" class="comic" data-group="preset">
    <div class="brand">
      <svg class="star" viewBox="0 0 24 24" aria-hidden="true">
        <polygon points="12,1 14.5,8.5 22,9 16,14 18,22 12,17.5 6,22 8,14 2,9 9.5,8.5"/>
      </svg>
      <div>
        <h1>KEITH</h1>
        <div class="sub">EDITOR PROTOTYPE · OFFLINE</div>
      </div>
    </div>
    <div class="strip-body">
      <div class="family-chips" id="families">
        <button class="chip active" data-id="preset_family" data-v="1">Family 01</button>
        <button class="chip" data-id="preset_family" data-v="2">Family 02</button>
        <button class="chip" data-id="preset_family" data-v="3">Family 03</button>
        <button class="chip" data-id="preset_family" data-v="4">Family 04</button>
      </div>
      <div class="lists">
        <div class="listbox" id="banks" data-id="preset_bank" title="Bank">
          <div class="row sel">Bank A  -  Factory</div>
          <div class="row">Bank B  -  User</div>
          <div class="row">Bank C  -  Palette Demo</div>
        </div>
        <div class="listbox" id="patches" data-id="preset_patch" title="Patch">
          <div class="row sel">001 Dry Kick Kit</div>
          <div class="row">002 Thin EP Stab</div>
          <div class="row">003 Clav Chase</div>
          <div class="row">004 Tape Bass</div>
          <div class="row">005 Off-Kilter Lead</div>
        </div>
      </div>
      <div class="btn-row">
        <button class="btn ghost" data-id="preset_prev">Prev</button>
        <button class="btn ghost" data-id="preset_next">Next</button>
        <button class="btn" data-id="preset_save">Save</button>
        <button class="btn mag" data-id="preset_init">Init</button>
        <button class="btn ghost" data-id="preset_favorite" id="favBtn">Fav</button>
      </div>
    </div>
  </section>

  <div id="main">

    <!-- DRUM -->
    <section id="drum" class="comic zone" data-group="drum">
      <div class="titlebar green">Drum Kit</div>
      <div class="body">
        <div class="pad-grid" id="pads"></div>
        <div class="block">
          <h3>Slot · <span id="slotLabel">01 Kick</span></h3>
          <div class="knob-row" id="slotKnobs"></div>
          <div style="margin-top:10px;display:flex;gap:16px;align-items:center;">
            <label class="lbl" style="font-size:11px;">MUTE <span class="switch" data-id="drum_slot_mute"><span class="thumb"></span></span></label>
            <label class="lbl" style="font-size:11px;">SOLO <span class="switch" data-id="drum_slot_solo"><span class="thumb"></span></span></label>
          </div>
        </div>
        <div class="block">
          <h3>Kit</h3>
          <div class="knob-row" id="kitKnobs"></div>
        </div>
        <p class="meta">Pads map MIDI 36 - 51 · see docs/DRUM-MAP.md (proposed)</p>
      </div>
    </section>

    <!-- SYNTH -->
    <section id="synth" class="comic zone" data-group="synth">
      <div class="titlebar gold">Synth / Bass / Keys</div>
      <div class="body">
        <div class="engine-row">
          <span class="lbl">Engine</span>
          <div class="seg" id="engineSeg">
            <button class="active" data-id="synth_engine" data-v="hybrid">Hybrid</button>
            <button data-id="synth_engine" data-v="analog">Analog</button>
          </div>
          <span class="lbl">Poly</span>
          <span class="switch on" data-id="synth_poly_mode"><span class="thumb"></span></span>
        </div>
        <div class="block">
          <h3>Osc</h3>
          <div class="knob-row" id="oscKnobs"></div>
        </div>
        <div class="block">
          <h3>Filter</h3>
          <div class="knob-row" id="filtKnobs"></div>
        </div>
        <div class="block">
          <h3>Amp Env</h3>
          <div class="knob-row" id="ampKnobs"></div>
        </div>
        <div class="block">
          <h3>Mod · Voice</h3>
          <div class="knob-row" id="modKnobs"></div>
        </div>
        <p class="meta">Neutral labels only · behaviour targets are clean-room (BRIEF §4)</p>
      </div>
    </section>

    <!-- FX -->
    <section id="fx" class="comic zone" data-group="fx">
      <div class="titlebar magenta">FX</div>
      <div class="body">
        <div class="block">
          <h3>Tape Bus</h3>
          <div class="knob-row" id="tapeKnobs"></div>
          <div style="margin-top:8px;">
            <span class="lbl" style="font-size:11px;">BYPASS </span>
            <span class="switch mag" data-id="fx_tape_bypass"><span class="thumb"></span></span>
          </div>
        </div>
        <div class="block">
          <h3>Insert 1 · <span style="color:var(--paper);font-family:'Space Mono',monospace;font-size:11px;">Algo 01</span></h3>
          <div class="knob-row" id="ins1Knobs"></div>
        </div>
        <div class="block">
          <h3>Insert 2 · <span style="color:var(--paper);font-family:'Space Mono',monospace;font-size:11px;">Algo 01</span></h3>
          <div class="knob-row" id="ins2Knobs"></div>
        </div>
        <div class="block">
          <h3>Master · <span style="color:var(--paper);font-family:'Space Mono',monospace;font-size:11px;">Algo 01</span></h3>
          <div class="knob-row" id="mstKnobs"></div>
          <div class="knob-row" id="outKnobs" style="margin-top:6px;"></div>
        </div>
        <p class="meta">Tape = global bus · insert/master macros proposed</p>
      </div>
    </section>

  </div>
</div>
<div class="size-readout" id="sizeReadout"></div>

<script>
(function () {
  const slots = [
    {n:"Kick", note:"C1"}, {n:"Snare", note:"C#1"}, {n:"Rim", note:"D1"}, {n:"Clap", note:"D#1"},
    {n:"CHH", note:"E1"}, {n:"OHH", note:"F1"}, {n:"PHH", note:"F#1"}, {n:"TomL", note:"G1"},
    {n:"TomM", note:"G#1"}, {n:"TomH", note:"A1"}, {n:"Ride", note:"A#1"}, {n:"Crash", note:"B1"},
    {n:"Perc1", note:"C2"}, {n:"Perc2", note:"C#2"}, {n:"Perc3", note:"D2"}, {n:"Perc4", note:"D#2"}
  ];

  // Visual order: bottom row first in DOM? We paint top-to-bottom as Row3..Row0
  const visualOrder = [12,13,14,15, 8,9,10,11, 4,5,6,7, 0,1,2,3];
  const padsEl = document.getElementById("pads");
  let selected = 0;

  function renderPads() {
    padsEl.innerHTML = "";
    visualOrder.forEach(function (idx) {
      const s = slots[idx];
      const pad = document.createElement("button");
      pad.className = "pad" + (idx === selected ? " selected" : "");
      pad.dataset.id = "drum_pad_" + String(idx + 1).padStart(2, "0");
      pad.innerHTML = '<span class="name">' + s.n + '</span><span class="note">' + s.note + '</span>';
      pad.addEventListener("pointerdown", function () {
        selected = idx;
        document.getElementById("slotLabel").textContent =
          String(idx + 1).padStart(2, "0") + " " + s.n;
        pad.classList.add("hit");
        setTimeout(function () { pad.classList.remove("hit"); }, 80);
        renderPads();
      });
      padsEl.appendChild(pad);
    });
  }

  function makeKnob(id, label, angle, mag) {
    const wrap = document.createElement("div");
    wrap.className = "knob-wrap";
    wrap.innerHTML =
      '<div class="knob' + (mag ? " mag" : "") + '" data-id="' + id + '" data-angle="' + angle + '">' +
      '<div class="tick" style="transform:translate(-50%,-100%) rotate(' + angle + 'deg)"></div></div>' +
      "<label>" + label + "</label><div class=\"val\"> - </div>";
    const knob = wrap.querySelector(".knob");
    let dragging = false, lastY = 0, ang = angle;
    function setAng(a) {
      ang = Math.max(-135, Math.min(135, a));
      knob.querySelector(".tick").style.transform =
        "translate(-50%,-100%) rotate(" + ang + "deg)";
      knob.dataset.angle = String(ang);
      const pct = Math.round(((ang + 135) / 270) * 100);
      wrap.querySelector(".val").textContent = pct + "%";
    }
    setAng(angle);
    knob.addEventListener("pointerdown", function (e) {
      dragging = true; lastY = e.clientY; knob.setPointerCapture(e.pointerId);
    });
    knob.addEventListener("pointermove", function (e) {
      if (!dragging) return;
      setAng(ang - (e.clientY - lastY) * 1.2);
      lastY = e.clientY;
    });
    knob.addEventListener("pointerup", function () { dragging = false; });
    knob.addEventListener("dblclick", function () { setAng(-20); });
    return wrap;
  }

  function fill(id, items, mag) {
    const el = document.getElementById(id);
    items.forEach(function (it) {
      el.appendChild(makeKnob(it[0], it[1], it[2], mag));
    });
  }

  fill("slotKnobs", [
    ["drum_slot_level","Level",-20],
    ["drum_slot_pan","Pan",0],
    ["drum_slot_tune","Tune",0],
    ["drum_slot_decay","Decay",40],
    ["drum_slot_filter","Filter",80],
    ["drum_slot_drive","Drive",-135]
  ]);
  fill("kitKnobs", [
    ["drum_kit_level","Kit Lvl",-20],
    ["drum_kit_swing","Swing",-135]
  ]);
  fill("oscKnobs", [
    ["synth_osc1_shape","Osc1",-100],
    ["synth_osc1_pitch","Pitch",0],
    ["synth_osc1_level","Lvl1",100],
    ["synth_osc2_shape","Osc2",-80],
    ["synth_osc2_pitch","Pitch2",20],
    ["synth_osc2_level","Lvl2",60],
    ["synth_sub_level","Sub",-135],
    ["synth_noise_level","Noise",-135],
    ["synth_fm_amount","FM",-135]
  ]);
  fill("filtKnobs", [
    ["synth_filter_cutoff","Cutoff",40],
    ["synth_filter_reso","Reso",-100],
    ["synth_filter_env","F Env",-20]
  ]);
  fill("ampKnobs", [
    ["synth_amp_attack","A",-120],
    ["synth_amp_decay","D",-40],
    ["synth_amp_sustain","S",40],
    ["synth_amp_release","R",-20],
    ["synth_master_level","Level",-20]
  ]);
  fill("modKnobs", [
    ["synth_porta","Porta",-135],
    ["synth_unison_voices","Unison",-135],
    ["synth_unison_detune","Detune",-60],
    ["synth_lfo_rate","LFO",-80],
    ["synth_lfo_amount","LFO Amt",-135],
    ["synth_filt_attack","F Att",-110],
    ["synth_filt_decay","F Dec",-40],
    ["synth_filt_sustain","F Sus",-20],
    ["synth_filt_release","F Rel",-20]
  ]);
  fill("tapeKnobs", [
    ["fx_tape_sat","Sat",-60],
    ["fx_tape_bump","Bump",-80],
    ["fx_tape_wow","Wow",-110],
    ["fx_tape_flutter","Flutter",-110],
    ["fx_tape_hiss","Hiss",-120],
    ["fx_tape_print","Print",-135],
    ["fx_tape_mix","Mix",-20]
  ], true);
  fill("ins1Knobs", [
    ["fx_ins1_a","A",0],["fx_ins1_b","B",0],["fx_ins1_c","C",0],["fx_ins1_mix","Mix",100]
  ], true);
  fill("ins2Knobs", [
    ["fx_ins2_a","A",0],["fx_ins2_b","B",0],["fx_ins2_c","C",0],["fx_ins2_mix","Mix",100]
  ], true);
  fill("mstKnobs", [
    ["fx_mst_a","A",0],["fx_mst_b","B",0],["fx_mst_c","C",0],["fx_mst_mix","Mix",-20]
  ], true);
  fill("outKnobs", [["fx_out_level","Out",-20]], true);

  renderPads();

  document.querySelectorAll(".switch").forEach(function (sw) {
    sw.addEventListener("click", function () {
      sw.classList.toggle("on");
    });
  });

  document.querySelectorAll("#families .chip").forEach(function (chip) {
    chip.addEventListener("click", function () {
      document.querySelectorAll("#families .chip").forEach(function (c) { c.classList.remove("active"); });
      chip.classList.add("active");
    });
  });

  document.querySelectorAll(".listbox").forEach(function (box) {
    box.addEventListener("click", function (e) {
      const row = e.target.closest(".row");
      if (!row) return;
      box.querySelectorAll(".row").forEach(function (r) { r.classList.remove("sel"); });
      row.classList.add("sel");
    });
  });

  document.querySelectorAll("#engineSeg button").forEach(function (btn) {
    btn.addEventListener("click", function () {
      document.querySelectorAll("#engineSeg button").forEach(function (b) { b.classList.remove("active"); });
      btn.classList.add("active");
    });
  });

  const fav = document.getElementById("favBtn");
  let favOn = false;
  fav.addEventListener("click", function () {
    favOn = !favOn;
    fav.textContent = favOn ? "Fav ★" : "Fav";
    fav.style.background = favOn ? "var(--gold)" : "";
    fav.style.color = favOn ? "var(--ink)" : "";
  });

  function size() {
    document.getElementById("sizeReadout").textContent =
      window.innerWidth + " × " + window.innerHeight + " · design 1280×800";
  }
  window.addEventListener("resize", size);
  size();
})();
</script>
</body>
</html>
'@

$html = $html.Replace("%%BANGERS%%", $bangers.Trim())
$html = $html.Replace("%%MONOREG%%", $monoReg.Trim())
$html = $html.Replace("%%MONOBOLD%%", $monoBold.Trim())

[IO.File]::WriteAllText($out, $html, [Text.UTF8Encoding]::new($false))
Write-Host "Wrote $out ($((Get-Item $out).Length) bytes)"
