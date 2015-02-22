# unsplash-retriever
A console utility for downloading images from [unsplash.com]

[unsplash.com]: https://unsplash.com

##Usage

Just run ``unsplash-retriever [options]`` (or .exe on Windows)


## Resuming downloads
For resuming downloads you can use the ``-r`` option. Download info is stored in a file named ``downloaded.dat`` so make sure to not remove it while using the utiliy.

##Options

Below are the supported options

``
  -h, --help                Displays this help.
  -v, --version             Displays version information.
  -q, --quiet               Hides messages and progress bars
  -f, --force               Overwrite existing files. By default download is
			    cancelled when a existing photo is found on disk
  -t, --target <directory>  Copy download photos to <directory>.
  -r, --resume              Try to resume download. Reads downloaded.dat
``

## Requirements for building
* Linux (Windows support is WIP)
* [qmake]
* [Qt 5.x] Development packages 
* [libxml2] development packages

[qmake]: http://doc.qt.io/qt-5/qmake-manual.html
[Qt 5.x]: http://www.qt.io/download/
[libxml2]: http://xmlsoft.org/

## Building

1. Run qmake
2. ./unsplash-retriever
3. Wait until downloads the whole repo
 
 
## Contributing

Do the usual GitHub fork and pull request dance.

## License

Released under the MIT license.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.