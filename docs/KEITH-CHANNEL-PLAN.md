# Keith channel in the Paper Trail server

Keith talks to a public Sam directly, in one channel of the Paper Trail server, about the MAGS
SYNTH interface. Sam posts the wireframes, Keith answers with his own screenshots. Noah stops
being the relay.

## Who answers

`sam-pt`, the isolated public profile. It carries its own Discord bot token, so its messages never
collide with the private Sam's, and it holds clean scoped memory rather than the private store.

Not the private profile. That one holds family, the other projects, the hardware and the business.
One config slip puts all of it in a room with Keith.

## What `sam-pt` has to be loaded with

The MAGS context, and nothing wider.

- `resources/reference/` and the owner's reference image
- the wireframe outputs in `design/screenshots/`
- the briefs in `briefs/`
- `docs/PARAM-MANIFEST.md`, the verified parameter list
- `docs/DECISIONS.md` and `docs/DEMO-STATE.md`
- the verified oscillator control hierarchy

## Steps

1. Invite `sam-pt`'s bot to the Paper Trail server, into one channel only, say `#mags-synth`.
   Needs Manage Server, or an invite from whoever has it.
2. Set channel permission overwrites so only Noah, Keith and the bot can see it. Paper Trail
   members do not.
3. In `sam-pt`'s config, set `discord.allowed_channels` to that channel ID alone.
4. Load the MAGS context above.
5. Restart `sam-pt`'s gateway. It has its own service and its own token.
6. The brake. Clear `allowed_channels` or remove the bot from the server and Keith's line is dead.

## Where `sam-pt` runs

ai-chud, not the Mac. A laptop goes dark on lid close and Keith messages whenever he likes. Discord
is an outbound WebSocket, so a second gateway conflicts with nothing and costs no GPU while
`sam-pt` answers with a cloud model.

## Open decisions

- Who invites the bot, Noah or whoever holds server permissions.
- Which model `sam-pt` answers with in this channel.
- Whether the channel is visible to Paper Trail staff or limited to Noah and Keith.
- Reach. The agent session cannot SSH to ai-chud today. This Mac's key is offered and ai-chud
  refuses it, and the session cannot answer a password prompt. `ssh-copy-id ai-chud` from Noah's
  own terminal resolves it.
