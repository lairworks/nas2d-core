// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

// This file is purely for documentation purposes and should contain no actual code.

/**
 * \mainpage
 * 
 * \note This is still a work in progress.
 * 
 * NAS2D is the open-source, object oriented 2D framework that powers <I>The Legend of Mazzeroth</I>. It's a flexible, extensible,
 * genre agnostic, easy to use framework for building 2D games.
 * 
 * \section overview A General Overview
 * 
 * NAS2D was designed with modularity, simplicity and portability in mind. To those ends, NAS2D provides a simple, consistent
 * and intuitive interface.
 * 
 * \section design-elements NAS2D Design Elements
 * 
 * NAS2D is broken up into several 'modules', each implementing specific functionality. This documentation currently covers use of the
 * Core module.
 *
 * \section getting-started Getting Things Rolling
 * 
 * After setting up your project and environment, the first thing you'll want to do is instantiate a Game object. The Game
 * object takes care of getting all utilities started and functional.
 * 
 * After creating a Game object, the \c Game::go() function is called with a pointer to a state object and starts the main
 * game loop. See State on how to derive new states.
 * 
 * The framework runs as a hybrid event-driven application (hybrid in that it doesn't wait for events but responds to them).
 * The \c Event System (implemented as an Observer Pattern loosely modeled on Qt's signals/slots implementation) handles all
 * events that the game might encounter including all input events (keyboard/mouse/joystick), window manager events (gained
 * focus, lost focus, minimized, maximized, etc.) and any low-level system events (namely quit requests). These events are
 * 'pushed' through the \c Gui if it has focus and then to the current State if the \c Gui isn't in focus.
 * 
 * Any object that wants to receive events must inheirt from a Listener object. This requires that the object implement the
 * push(Event* event) function. All GUI Controls (known as widgets for anybody familiar with Apple/Linux lingo) inherit from
 * Listener as do all State objects.
 * 
 * As the whole system is implemented as a hybrid event-driven system, even if there are no events in the event queue the
 * program will continue to run by calling the update() function of all objects that are in memory (states, GUI Controls,
 * etc.). This is handled automatically and should not be called explicitly by the programmer.
 * 
 * \section state-machine StateManager and the Finite State Machine Model
 * The Finite State Machine Model is a commonly used model to control logical states in games. They can be implemented in
 * many different ways and in the frameworks case the states are used to organize game logic into distinct units. In the
 * initial code for the framework, SplashScreenState is an example of a Finite State which handles a 'splash screen' of sorts.
 * It draws a background image on the screen, draws some text and adds some Windows to the GUI (the windows are built-in UI
 * types.)
 * 
 * To that end, all game states should be implemented as an object that derives from State
 * (defined in src/Utility/StateManager.h). Derived states are generally stored in the src/Game/ folder to make it easy to
 * find them.
 * 
 * When a State object is derived, it needs to implement the following four virtual functions:
 * 
 * \code
 * void initialize();
 * void finalize();
 * State* update();
 * void push(Event *event);
 * \endcode
 * 
 * The initialize() function is used to initialize any member variables that the state declares (such as Image, Font, Sound, Music, etc.). initialize() can be though of as the Constructor for a state. States should not use a Constructor or Destructor to initialize/free member variables as the services such as the Renderer, Mixer, GUI, etc. are not yet initialized until the State is added to a StateManager.
 * 
 * The update() function is called every frame by the state manager. This is the function where game logic should be implemented. It must return a pointer to a State object. Generally the 'this' pointer is returned. A new State can be returned (e.g., return new GameState()) or NULL can be returned. Returning NULL will cause the game to shut down and quit.
 * 
 * The push(Event* event) function is the function where event's are passed through. This is where responses to input events should be handled. SplashScreenState responds to several event messages and can be used as a simple example.
 * 
 * \section filesystem The Filesystem
 * The \c Filesystem in LoM is implemented virtually. This means that we define where the program will look for its files, where it's allowed to write files and how the filesystem appears to the program. It also lets us pack all the files up into ZIP archives and load them dynamically that way -- it's all transparent to the user and to the program.
 * 
 * The service is started by the \c Game object as a \c Singleton. Whenever \c Filesystem services are needed one should request a reference to it using the Singleton object: \c Singleton<Filesystem>::get()
 * 
 * NOTE: The Filesystem expects file paths to use the forward slash '/' character instead of the backslash '\' character that's used in Windows. This is to keep things portable.
 * 
 * \section resources Resources and Assets
 * All art assets like sound, music, images, fonts and animated sprites are called Resources. They are all derived from a Resource object.
 * 
 * All resources can be initialized as a copy of another matching Resource:
 * 
 * \code
 * Image image1("graphics/sys/splash.png");
 * Image image2(image1);
 * Image image3;
 * image3 = image1;
 * \endcode
 */