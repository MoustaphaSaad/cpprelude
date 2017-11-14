# CPPrelude

**CPPrelude** is a body of code that I tend to write over and over again in every project I make. So, I decided to write it as a library and include it in my projects.

It contains algorithms, containers, memory management stuff ... etc.

Developed by:

**[Moustapha Saad Abdel-hameed](moustapha.saad.abdelhamed@gmail.com)**

**[Nora Abdel-hameed](nora.abdelhameed@gmail.com)**

**CPPrelude** version 1.0.0 is released under the GNU Lesser General Public License version 3.

## Philosophy and Coding standard

For Philosophy and coding standard please refer to this [link](https://moustaphasaad.github.io/2017/08/05/cpprelude/).

## Building

1. **CPPrelude** uses premake5 as its meta build system so you have to download premake5.
2. Use premake5 to generate project/solution files in order to compile the library.
   1. Ex: `premake5 gmake` this will generate the build folder with Linux make files inside.
   2. Ex: `premake5 vs2015` this will generate the build folder with visual studio 2015 solution files inside.
3. **CPPrelude** supports 32-bit and 64-bit targets and has a *debugShared* and *releaseShared* modes.
4. Building the library is as easy as invoking your build system.
5. Binaries should be generated inside a bin folder right beside the build folder,
6. Also, a unittest executable will be generated. You can invoke it to test your **CPPrelude** build.


## Content

|                 Content                  |               Description                |
| :--------------------------------------: | :--------------------------------------: |
|         [Types](Types/Types.md)          | Contains the primitive types that cpprelude uses |
|        [Memory](Memory/Memory.md)        | Contains the memory management model (slices, alloc, virtual_alloc, free, ...) |
|  [Containers](Containers/Containers.md)  | Contains variety of containers (dynamic_array, slinked_list, queue, tree_map, hash_array, ...) |
|  [Algorithms](Algorithms/Algorithms.md)  | Contains variety of algorithms (shuffle, mergesort, quicksort, quickselect ...) |
|  [Allocators](Allocators/Allocators.md)  | Contains a wrapper to allocation algorithms |
| [Memory Context](Allocators/memory_context.md) | Contains a memory context for allocators |
|     [Platform](Platform/platform.md)     |       Contains the platform layer        |
| [Memory Watcher](Platform/memory_watcher.md) | Contains a memory watcher for detecting memory leaks |
|   [Threading](Threading/Threading.md)    | Contains useful wrappers that help in thread safe operations |

## How to contribute

Feel free to fork and create pull requests at any time. It would be nice if you involved any one of us with you from the start to guide you, or maybe discuss your idea itself (Developers e-mails are up above).
