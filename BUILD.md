# Building VecFX

Nominally, VecFX is included as part of Mo++, and is automatically compiled along with the rest of Mo++'s libraries. However, if you choose to work on it separately, you can compile it like so:

## Getting the sources

Make sure you have `pugixml` installed. You can find its installation instructions at [its website](https://pugixml.org/).

Clone the repo from github:

```
git clone https://github.com/PhantomzBack/VecFX.git && cd VecFX
```

## Compiling

From there, assuming you have `GNU make` and the `g++` compiler, all you need to do is

```
make
```

For any other compiling options, run `make help`, which will list all compiling options.
