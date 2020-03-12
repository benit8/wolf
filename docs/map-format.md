# Map format
This file describes the binary data contained inside the maps of this game.

The file extension will be `w3d`, even though no check will be done.

|Field name      |Length          |
|----------------|----------------|
|Map width       |2               |
|Map height      |2               |
|Map title length|1               |
|Map title       |`title_length`  |
|Map data        |`width`*`height`|
|Player initial X|2               |
|Player initial Y|2               |