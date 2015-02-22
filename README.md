# unsplash-retriever
A console utility for downloading images from [unsplash.com]

[unsplash.com]: https://unsplash.com

##Usage

Just run ``unsplash-retriever [options]`` (or .exe on Windows)


## Resuming downloads
For resuming downloads you can use the ``-r`` option. Download info is stored in a file named ``downloaded.dat`` so make sure to not remove it while using the utiliy.

##Options

Below are the supported options

* ``-q, --quiet               Hides messages and progress bars``
* ``-f, --force               Overwrite existing files. By default download is			    cancelled when a existing photo is found on disk``
* ``-t, --target <directory>  Copy download photos to <directory>.``
* ``-r, --resume              Try to resume download. Reads downloaded.dat``

## Requirements for building
* [Visual Studio] (on windows)
* [qmake]
* [Qt 5.x] Development packages 
* [libxml2] development packages

[Visual Studio]: http://www.visualstudio.com/
[qmake]: http://doc.qt.io/qt-5/qmake-manual.html
[Qt 5.x]: http://www.qt.io/download/
[libxml2]: http://xmlsoft.org/

## Building

### Linux

1. ``cd /path/to/unsplash-retriever``
2. Run ``qmake``
3. Run ``make``
4. Run ``./release/unsplash-retriever -t ~/somedir``

### Windows

1.  Open the [Developer Command Prompt]
2. ``cd /path/to/unsplash-retriever``
3. ``qmake``
4. ``nmake``
5. Run ``release/unsplash-retriever.exe -t somedir``
 
[Developer Command Prompt]: https://msdn.microsoft.com/en-us/library/ms229859%28v=vs.110%29.aspx
 
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
