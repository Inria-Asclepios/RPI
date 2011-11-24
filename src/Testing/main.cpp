
#include "mainWindow.h"

#include <QApplication>

int main(int argc, char * argv[])
{
    QApplication theApp(argc, argv);

    //const std::string inputFilename = "c:/Temp/brebix.mha";
    //const std::string inputFilename = "c:/Temp/testfile.mha";
    //const std::string inputFilename = "C:/Users/jstark/Projects/sampleData/AMC_2011-11/Ex365Em505T12000ms.nii";
    if (argc > 2 ) {
        std::cerr << "Usage " << argv[0] << " filename" <<std::endl;
        return 1;
    }

    MainWindow mw;

    mw.show();

    if ( argc == 2 ) {
        const QString inputFilename = argv[1]; //"C:/Users/jstark/Projects/sampleData/AMC_2011-11/pig in ii.nii";
        mw.setFilename(inputFilename);
    }
    return theApp.exec();

}
