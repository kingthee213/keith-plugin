# Layout direction, 2026-10-08

The interface questions, in the order they get decided. Settled items are settled by the owner.

## Settled

**Everything on the canvas must look physically possible.** The instrument reads as a physical machine, so
every element has to be something a machine could actually have. Knobs, switches, buttons and silkscreen
labels are all fine. A lit button embedded in the raw wood rail is not. This killed the idea of a module
picker on the wooden rail, which was the wrong answer for exactly that reason.

**The overall shape is B, grouped rows with a switcher per row.** Everything stays on screen, organised
into rows by what it does, and each row can hold more than one module with a switcher choosing which fills
it. This is closer to the owner's reference than the current build, not further from it. The reference is
already three rows; the switchers are what let each row carry more than one thing.

The switcher is a physical idea, so it presents as a row of buttons, the way a hardware synth pages a
section. It is not a menu and it is not on the wood.

**Drums and synth are not exclusive states.** They do not need a mode that hides one to play the other.
The pads answer MIDI 36 to 51 and the keyboard answers the rest, so a single instance already plays both at
once and one sequencer track of drum notes and one of synth notes can drive the same plugin. Making them
mutually exclusive would be a limitation invented for layout convenience, which is backwards. So the pads
are a performance surface that stays on screen next to the keyboard, and the row above them holds whatever
edits the pad you have selected.

## Recorded, not being worked on yet

The owner's verdict on the drums, so it is not lost. Kick, snare, rim and toms are fine. The cymbals are
bad. About 70 percent of the samples are the same as each other, and there is not enough variety overall.
`docs/DRUM-SOUND-SOURCING-PLAN.md` is the plan for fixing that and it waits on the licensing decision it
already names. The owner said drums come back later, so nothing here is actioned.

Separate from the drums, and its own finding: the synth feels limited in pitch range when played from the
keyboard. That is an engine question rather than a layout one and it is not yet investigated.

## Open

The row grouping. What goes in each row, and which modules share a row.
