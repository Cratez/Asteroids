
To configure the SFML for this project to your SFML binaries

For example,
If your solution dir is D:\Dev\Projects\Droids, and inside that your project dir is D:\Dev\Projects\Droids\Droids
Then you SFML binaries folder should be at D:\Dev\Projects\SFML

Inside that SFML folder, the following should exist
D:\Dev\Projects\SFML\include
D:\Dev\Projects\SFML\lib
D:\Dev\Projects\SFML\bin

The include/lib directories should referenced as a relative path, so just ensure that your sfml folder is in the same folder where your solution folder is.

Additional Note that I am only compiling for x64
