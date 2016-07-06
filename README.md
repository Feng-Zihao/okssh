okssh
===

console GUI tool to manage ssh

Gif record
---
![screenrecord](https://raw.githubusercontent.com/Feng-Zihao/okssh/master/screenrecord.gif)

Usage
---
`oksssh` and you'll find out how.

config.xml
---
Create a default file `~/.okssh/config.xml`.

Similar with `~/.ssh/config`, but more structural robust.

```
hosts:
    -
        description: example description
        keyfile: /absolute/path/for/keyfile
        target: user@hostname

```

This is designed for later extension such as `executing some command right after logged in`, `using a keyfile by a HTTP URL`.


Useful Link to implement
---
[cursor movement](http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html)

[ncurses](http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)

[colors and flags in terminal](http://askubuntu.com/questions/558280/changing-colour-of-text-and-background-of-terminal)
