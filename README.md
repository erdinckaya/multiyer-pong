# Pong

Server Authoritative Multiplayer Pong Game.

This project is following `Data Oriented Desing Paradigm`, uses `UDP` and `RUPD` to communicate between server and client by using `Yojimbo`, and uses `SDL2` and `OpenGL` for rendering and input handling.

This project is designed as example project for these purposes.
 * Designing server authoritative multiplayer fast paced game with UDP and RUPD.
 * Using Snapshot Interpolation to control game states.
 * Implementing client and server in same code to prevent code duplicating.
 * Handling lags and package loss.
 * Creating simple 2D engine with OpenGL and SDL2
 * Using and understanding Data oriented paradigm.
 * Understanding simple principles of OpenGL such as drawing text drawing geometrical shapes and rendering texts.

 ### Dependencies
 * [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) or newer.
 * [vcpkg](https://github.com/microsoft/vcpkg)
 * First assign your `vcpkg.exe` path and run `install_dependencies.bat`
 * [Yojimbo](https://github.com/networkprotocol/yojimbo) for networking, however I have renamed and refactored some yojimbo macroes and functions so that if you want to change it you can pull that [repo](https://github.com/erdinckaya/yojimbo).

 ### Config

This project has four configurations.
 * `SERVER` => Creates Server build
 * `CLIENT` => Creates Client build
 * `ACTIVATE_BOT` => Add bot to player1 to play alone.
 * `EDITOR` => Opens an entity editor to change them on the fly.

Project is designed for using client and server code in the same repository so we need a configuration 
to distinguish them. `CLIENT` and `SERVER` configs are doing this job, therefore you cannot use both of them 
in the same configuration. However you are able to use other configurations freely.

Another obligatory is that If you want to use `ACTIVATE_BOT` config, you have to add this configuration both server
and client builds.

 ### Building

Open solution file with Visual Studio, and build project for Server and Client configuration, after that you can 
use that run.bat file to start server and two clients. 

 ### Usages

It is basic pong game, you can see all options with pushing `H` key. Here are options that you can change or observe.
 * Pause the game.
 * Restart the game.
 * Changing package loss for testing.
 * Changing latency for testing.
 * Changing jitter for testing.
 * Changing duplicate packages for testing.
 * Observing colliders.

For further informations you can read the code comments.