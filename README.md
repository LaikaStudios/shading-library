# Production Shading Library

[![](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE-APACHE)
[![](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE-MIT)

The Production Shading Library refers to [Laika's](https://www.laika.com) entire collection of VFX production shading capabilities. These are embodied primarily in the set of shading-library files. This repository will contain those portions of Laika's Production Shading Library that have been released as open source.

This release contains a set of shading nodes that provide a [Material Layering System](https://github.com/LaikaStudios/shading-library/wiki/dev.Material-Layering-System) based on [Pixar's](https://www.pixar.com) [RenderMan](https://renderman.pixar.com/product).

If you make use of this repsitory, I strongly urge you to [subscribe to the discussion group](https://groups.google.com/group/laikastudios-shading-library). Please use this group to provide feedback on any bugs you may find or to discuss any feature requests.

## Requirements
* [Pixar's](https://www.pixar.com/renderman) [RenderMan 23](https://rmanwiki.pixar.com/display/REN23/RenderMan) </br></br>
* [Foundry's](https://www.foundry.com) [Katana](https://www.foundry.com/products/katana)
* [Pixar's](https://www.pixar.com/renderman) [RenderMan 23 for Katana 3.2 or 3.5](https://rmanwiki.pixar.com/display/RFK23/RenderMan+23+for+Katana) </br>~and/or~</br>
* ~[Autodesk's](https://www.autodesk.com) [Maya](https://www.autodesk.com/products/maya/overview)~
* ~[Pixar's](https://www.pixar.com/renderman) [RenderMan 23 for Maya 2017/2018/2019](https://rmanwiki.pixar.com/display/RFM23/RenderMan+23+for+Maya)~ </br></br>
* A c++14 compatible compiler (clang++ is specified in the ris/Makefile)

## Documentation
Documentation is on the [wiki page](https://github.com/LaikaStudios/shading-library/wiki/dev.Home).

## License
Licensed under either of

 * Apache License, Version 2.0, ([LICENSE-APACHE](LICENSE-APACHE) or http://www.apache.org/licenses/LICENSE-2.0)
 * MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)

at your option.

### Contribution
Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be dual licensed as above, without any
additional terms or conditions.
