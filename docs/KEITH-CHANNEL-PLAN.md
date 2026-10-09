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

## Who does what

Noah, or whoever holds server permissions on the Paper Trail server.

1. Check `sam-pt` actually exists and runs somewhere first. Nothing can be invited until there is a
   bot to invite. Its state is not visible from the MacBook.
2. Discord Developer Portal, the application that owns `sam-pt`'s bot. OAuth2, URL generator.
   Scopes `bot` and `applications.commands`. Permissions, the least that works, View Channels, Send
   Messages, Send Messages in Threads, Attach Files, Read Message History, Add Reactions.
3. Open the generated URL, pick the Paper Trail server, authorize.
4. Make the channel, `#mags-synth`. In its permissions, deny View Channel for `@everyone`, then
   allow it for the bot and for Keith.
5. Copy the channel ID, with Developer Mode on, right click the channel, Copy Channel ID.

Sam, after step 5.

6. Set `discord.allowed_channels` on `sam-pt` to that one ID.
7. Load the MAGS context.
8. Restart the gateway and confirm by posting one wireframe into the channel.

## Where `sam-pt` runs

ai-chud, not the Mac. A laptop goes dark on lid close and Keith messages whenever he likes. Discord
is an outbound WebSocket, so a second gateway conflicts with nothing and costs no GPU while
`sam-pt` answers with a cloud model.

## Reach to ai-chud

The agent session cannot SSH to ai-chud. The cause is settled now. The Mac's key is already in
ai-chud's `authorized_keys` (`ssh-copy-id` reports all keys already installed), the key is
passphrase protected, and the agent the session talks to holds no identities, so a non-interactive
command has no way to supply the passphrase. One line in Noah's own terminal fixes it.

```bash
ssh-add --apple-use-keychain ~/.ssh/id_ed25519
```

This is not needed for the plugin work. Everything for the plugin happens on the MacBook.

## Open decisions

- Who invites the bot, Noah or whoever holds server permissions.
- Which model `sam-pt` answers with in this channel.
- Whether the channel is visible to Paper Trail staff or limited to Noah and Keith.
