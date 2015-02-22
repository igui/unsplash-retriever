#include <QCoreApplication>
#include <QTimer>
#include "retriever.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("unsplash-retriever");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Downloads photos from unsplash.com");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption(QCommandLineOption({"q", "quiet"}, "Hides messages and progress bars"));
    parser.addOption(QCommandLineOption({"f", "force"}, "Overwrite existing files. By default download is cancelled when a existing photo is found on disk"));
    parser.addOption(QCommandLineOption({"t", "target"}, "Copy download photos to <directory>.", "directory"));
    parser.addOption(QCommandLineOption({"r", "resume"}, "Try to resume download. Reads downloaded.dat"));


    parser.process(a);

    auto destination = parser.value("t");

    if(!QDir(destination).exists())
    {
        qFatal("Destination dir %s doesn't exist.", qPrintable(destination));
        return 1;
    }

    Retriever *retriever = new Retriever(&a, !parser.isSet("q"), parser.isSet("f"), parser.isSet("r"), destination);

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(retriever, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, retriever, SLOT(run()));

    return a.exec();
}
