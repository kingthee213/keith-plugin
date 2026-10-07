# BRIEF.md — KEITH (Mishby foundation synth)

Working name: **Keith**. Repo: `D:/TYH_R10/dev/keith-plugin`.
Owner may rename; if the name changes, change it here first and nowhere else.

An instrument plugin built for one person. Handed over, never sold, never published.

---

## 0. HOW THIS IS BUILT

Never hand-built by an orchestrator agent. Builder lanes, each working from this brief plus the
repo rules:

- **Cursor (Composer)** — C++ plugin skeleton, DSP engine, voice architecture, FX, build system,
  macOS AUv2 + VST3 (universal), Windows VST3, CI.
- **Cursor (Grok)** — editor/UI, cartoon visual language, parameter mapping, preset browser.
- **OMP (DeepSeek Flash) workers** — bulk mechanical passes: parameter tables, sample-map
  manifests, bank and patch conversion, factory-list enumeration from documentation, docs,
  repetitive refactors.

Orchestrator only sequences lanes, verifies against Section 12 and reports. No lane ships an asset
or a code file whose origin and licence it cannot name.

Repo and dispatch rules:

- New repo, git, branch `main`, one writer at a time in the checkout.
- Worker briefs live in `briefs/` as `<unit>-<worker>-<slug>.md`; they are read from disk, never
  pasted into a terminal.
- Workers never mutate git. No add, commit, branch, push, rebase, force-push. The coordinator owns
  commits.
- A worker report is a claim until the coordinator or an independent reviewer reproduces it. Paste
  real output, never a description of output.

---

## 1. OBJECTIVE

One instrument plugin, built on the **Mishby by Freakshow Industries** as its sonic/character
foundation so analog capability/emulation is possible. It is an amalgamation of the sounds and
synths the owner named, as his own version to fine-tune and work with:

- drum sounds and factory content modelled on **Triton, Triton Extreme, Korg M1, Korg Wavestation**
- synth capabilities mimicking the **Surge synth** and **OB-XF**
- Triton-family sounds preloaded with the effects they have in the Tritons
- coverage of everything related to the **Neptunes**
- the **Chad Hugo sound kit by Splice** used as reference for our own banks only
- interface based on the cartoon style of **Gorillaz** and **Green Lanterns**

Success reads as: this is the plugin he reaches for first every time. Full workflow coverage in one
window is the design goal; this is not a bid to replace his DAW or his other tools.

---

## 2. IT IS AN INSTRUMENT, NOT A SEQUENCER — MIDI IN, SOUND OUT

- **End user:** one person (the owner's friend). Handed to him; not sold, not published.
- **Host:** Ableton Live 12 Suite. macOS is his daily driver; a Windows build ships too and is a
  first-class target, not a courtesy.
- **Formats:** AUv2 (macOS) + VST3 (macOS and Windows). Live supports VST3 and AU, so either works.
- **Deployment targets:** macOS 11.7.10 Big Sur and up; Windows 10 22H2 / Windows 11 and up. These
  are Live 12's own floors, so they are the floors here.
- **Binaries:** macOS universal (arm64 + x86_64) — his Mac model is unknown, so cover both.
  Windows x64.
- **Behaviour:** the plugin receives MIDI from the host track like any instrument. His keyboard or
  pad controller plays whatever the track is routed to. **No internal step sequencer, no pattern
  player, no Drum Rack bridge.** MIDI in, audio out.
- **Drum section:** multi-slot sample playback driven by MIDI, slots mapped across the keyboard,
  with a documented drum map so pads and keys trigger them. One kit per preset, velocity layers and
  round-robin so it does not machine-gun.
- 64-bit only. No online authorisation, no licence server, no telemetry, no analytics.
- Presets and banks are self-contained and portable: no absolute paths, no external sample
  directory required, banks travel with the plugin folder.

---

## 3. SONIC FOUNDATION (Mishby character)

Tape machine / analog degradation is the foundation voice: saturation, head bump, wow/flutter
drift, hiss, print-through, unstable pitch. Usable from subtle glue to full destruction, as a global
bus and as per-part FX.

**[L1]** Mishby by Freakshow Industries is closed source under a personal, non-transferable EULA;
its code is not available and cannot be a code foundation. Faithful path: reimplement the behaviour
in our own DSP, seeded from a permissively licensed tape chain — Airwindows tape code is MIT and
explicitly fine in closed-source code — then tuned by ear. No Freakshow code, no decompiled
Freakshow binaries in the tree.

---

## 4. SYNTH ENGINE (mimics Surge synth + OB-XF)

- **4.1 Surge-style:** wavetable + hybrid oscillators (FM/RM/PD), modulation matrix, its filter set.
- **4.2 OB-XF style:** classic subtractive voice — 2 osc + sub, poly portamento, OB-X ladder/sem
  filter behaviour, unison.
- **4.3 Both in one plugin:** selectable engine/voice architecture per part.

**[L2 — LOCKED]** Surge (`surge-synthesizer/surge`) and OB-Xf (`surge-synthesizer/OB-Xf`) are
GPL-3.0, and this plugin is handed to another person, which is distribution. Therefore:
**clean-room behaviour mimic only.** Both are study references — manuals, listening, matching by
ear. Ingest none of their source: no forks, no vendored files, no line-by-line porting. Same rule
for any other GPL or non-commercial synth used as a reference.

---

## 5. FRAMEWORK

**iPlug2, locked.** Grounded: iPlug2's licence is zlib-like — free to use in closed source
projects, commercial included — and it targets VST3, AUv2 and AUv3 on macOS and Windows. No
subscription, no revenue tier, no ongoing cost, which fits the owner's no-new-recurring-bills rule.

JUCE is out for this build: its free Personal tier is for a private individual on his own
non-commercial project, which is not this, and the paid tiers buy nothing we need.

Record the iPlug2 attribution line and its bundled permissive deps (WDL, NanoVG, NanoSVG, RTAudio,
RTMidi) in the repo's license notes.

---

## 6. CONTENT — DRUMS + FACTORY BANKS

Four families, each with drum sounds and factory programs, at full library scale: mirror the factory
program/combi/patch sets of the named synths as our own recreations. The content lane enumerates the
real factory lists from public documentation — manuals, patch lists, published program banks — logs
every one in `banks/SOURCES.md` with its source, and **reports the real totals before building. Do
not invent patch names or counts.**

**6.1 Build order (decided):** Triton first, then Triton Extreme, then Korg M1, then Korg
Wavestation.
Reason: Triton establishes the patch format and the FX-chain architecture and is the family named
most; Triton Extreme reuses that structure; M1 is a simpler PCM/AI engine; Wavestation's
wave-sequencing is an engine of its own, so it goes last.

**6.2 Phasing:** Triton family + engine + UI ship first as a usable v0.1 he can make beats with.
Remaining families land as bank updates — banks are data, so no rebuild is needed to add them. The
full library remains the goal; it is not a v0.1 blocker.

**6.3 Source of the sounds:** the owner has no hardware and no reference recordings. Content is
built from (a) public documentation of how the original machines worked — engine architecture, PCM
wave lists, FX algorithm names and routing — and (b) other digital recreations where the licence
actually permits reuse.

> **Guardrail.** A recreation being open source does not make its sample content reusable: the code
> licence and the content licence are separate questions. Ingest a sample or a patch only when its
> licence permits redistribution and the attribution is recorded. Where it does not, design our own
> recreation to match by ear and note that in `banks/SOURCES.md`.

**6.4** Triton-family programs ship preloaded with the effects they have in the Tritons — the
factory program's insert + master FX chain is embedded per patch and reproduced on load, not left
dry.

**6.5 Naming:** neutral family labels in the UI and in preset names (Family 01–04, or neutral names
the owner picks). The Korg names — Triton, Triton Extreme, Korg M1, Korg Wavestation — appear only in
the internal reference doc, **never in the UI, preset names, file names or plugin metadata.**

**[L3]** Korg ROM waves, Korg factory programs and the Korg names are Korg IP/trademarks. Neutral
labelling keeps a private handover clean and keeps him safe if he posts or streams with it on
screen.

---

## 7. FX

Triton-style preloaded per-patch chains (Section 6) plus the Section 3 tape/analog block as a global
bus. Everything automatable and saved inside presets. Strong enough that a beat of this style needs
no third-party plugins.

---

## 8. NEPTUNES COVERAGE

"Capable of doing anything related to the Neptunes" = the plugin covers the Neptunes production
palette end to end: clav and key stabs, thin soulful electric piano, hard dry drums, off-kilter
synth textures, heavy per-part FX, sparse arrangements.

Deliverable that proves it: (a) the Section 4 engine plus Section 6 banks can dial any such sound,
and (b) a curated preset bank that demonstrates it.

**[A1]** "Neptunes" = Pharrell Williams + Chad Hugo. No living artist's name, likeness or branding
inside the plugin.

---

## 9. CHAD HUGO SOUND KIT BY SPLICE

**Reference only, no exceptions.** No Splice samples inside the plugin, no Splice files in the repo,
no Splice content in any bank. The kit is listened to as a target, our own banks are built to match
it, each logged in `banks/SOURCES.md`.

**[L4]** Splice's licence permits use in music and forbids redistribution of raw samples or shipping
them in a sample library or plugin — this path stays clean by design, and matches the owner's
instruction.

---

## 10. INTERFACE

Cartoon visual language referencing **Gorillaz** and **Green Lanterns** (DC — the comics and the
animated show): thick ink lines, flat saturated colour, comic-panel energy, cosmic green/magenta/gold
palette. **Original art only.**

**[L5]** No Gorillaz characters or artwork (Jamie Hewlett), and no DC Green Lantern characters,
rings, lantern icons, corps symbols, logos or costume marks. Style reference only; new art drawn in
that school with nothing recognisable in it.

- Resizable, one window, every visible control maps 1:1 to a real parameter.
- Layout supports the first-call goal: drum kit, synth/bass/keys, FX and preset browser all
  reachable without a tab hunt.

---

## 11. DELIVERY TO THE FRIEND'S MACHINE (no Apple Developer account)

- The macOS build ships **unsigned and unnotarised**. Gatekeeper quarantines downloaded plugins and
  reports "damaged / cannot be opened". It is not damaged; it is the quarantine flag.
- `docs/INSTALL-MAC.md` carries the two real fixes: allow it in System Settings > Privacy &
  Security, and clear the flag with
  `xattr -dr com.apple.quarantine "<path to .vst3 or .component>"`.
- That note is **verified end to end on a real Mac** — a clean machine, an unzipped build, both AU
  and VST3 loading in Live 12 Suite — before handover. Log the exact steps that were needed in the
  verification log at the foot of the note.
- Windows: a VST3 copied to `C:\Program Files\Common Files\VST3` loads unsigned; no signing needed.
  `docs/INSTALL-WINDOWS.md`.
- Deliver both platforms as a zip with the install notes.

---

## 12. ACCEPTANCE / DEFINITION OF DONE

- Builds clean from CI: macOS universal AUv2 + VST3, Windows x64 VST3. CI matrix: an Apple silicon
  mac runner, an Intel mac runner, and a Windows runner. **[A2]** Intel mac runners may be
  unavailable on the CI provider; if so the Intel-leg check moves to a local machine and the result
  is logged, never silently skipped.
- `pluginval` passes at strictness 10 on macOS and Windows.
- AU passes `auval -v aumu <subtype> <manufacturer>` on macOS. **[A3]** auval can be flaky headless
  in CI; if it is, it runs on a real Mac and the output is pasted into the repo log.
- Loads and plays in Ableton Live 12 Suite on macOS (both AU and VST3) and on Windows (VST3),
  receives MIDI from a keyboard and a pad controller, and the documented drum map is verified by
  hand on both.
- Deployment target honoured: runs on macOS 11.7.10 and Windows 10 22H2.
- No crash and no click on preset change or parameter change, including during playback.
- Family 01 (Triton-family) loads with its FX chains preloaded, spot-checked by ear and logged
  against the reference documentation. Repeats for each family as it lands.
- Preset save/load round-trips exactly: parameters + sample map + preloaded FX chain. Live set
  save/reopen restores state exactly.
- One instance runs a full arrangement at an agreed CPU budget with no dropouts; the plugin loads in
  a few seconds with content streamed rather than fully preloaded.
- The Neptunes preset bank (Section 8) exists and covers the listed palette.
- `banks/SOURCES.md` covers every bank and every patch. License audit signs off that the tree
  contains no Korg ROM content, no Splice samples, no Freakshow code, no GPL synth source, and no
  Gorillaz or Green Lantern assets. Neutral labels confirmed: no Korg names anywhere in UI, preset
  names, file names or plugin metadata.
- README, install steps for both OSes, and a preset/control manual including the drum map.

---

## 13. OPEN ITEMS

- Product name is working-only: **Keith**. Confirm or replace, then update this header.
- Everything else is locked. No lane waits on a decision.
