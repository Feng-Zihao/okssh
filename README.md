okssh
===

console GUI tool to manage ssh

Gif record
---
![screenrecord](https://raw.githubusercontent.com/Feng-Zihao/okssh/master/screenrecord.gif)

Usage
---
`okssh` and you'll find out how.

config.yaml
---
Create a default file `~/.okssh/config.yaml`.

Similar with `~/.ssh/config`, but more structural robust.

```
hosts:
    -
        description: example description
        keyfile: /absolute/path/for/keyfile
        target: user@hostname

```


Useful Link to implement
---
[cursor movement](http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html)

[colors and flags in terminal](http://askubuntu.com/questions/558280/changing-colour-of-text-and-background-of-terminal)
