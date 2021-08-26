# kleinsHTTP: A stupid stupidly simple http server.

## About

I wanted to create an api server using C++ as it is my favorite language however i was unable to find a http libary that fits my requirements.

Those requirements being:
- Simple to understand
- Simple to get working
- Easy to implement

So i wrote this libary. I don't know if this makes sense to other people, but it works for me.

## Building

You can build either in debug or release mode using

```make```

or

``make release``

respectivly.

This will create all necessary files under `target/debug|release/`.

### Linking it staticly

Include kleinsHTTP.h in your project.
Add kleinsHTTP.a to your source files.

### Linking it dynamicly

Include kleinsHTTP.h in your project.
Move libkleinsHTTP.so to `/usr/lib/`

## Example:

Check out [example.cpp](./example.cpp) on how to use this libary.

(I'm still working on better documentaion. Sorry!)

## FAQ:

### Do accept code contributions:

YES! Any help would be greatly appritiated.

### I don't like the name.

That is not a question. But me neither.

If you have a better idea for a name plase create a issue with the suggestion.

### How can i contact you?

Make an issue or contact me via Discord (kleins#9003) / [Twitter (@kleinschrader)](https://twitter.com/kleinschrader)

### Does this libary support HTTPS?

Yes, check the example for more info.

### Can i use this project to serve static files.

Yes! Checkout [kleins::httpServer::serveDirectory](source/httpServer/httpServer.h:96)

### Is there a docker image to base my project of?

Yes. Its [kleinschrader/kleinsHTTP](https://hub.docker.com/repository/docker/kleinschrader/kleinshttp/)
