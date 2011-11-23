
#include "mainWindow.h"

#include <QApplication>

int main(int argc, char * argv[])
{
    QApplication theApp(argc, argv);

    //const std::string inputFilename = "c:/Temp/brebix.mha";
    //const std::string inputFilename = "c:/Temp/testfile.mha";
    //const std::string inputFilename = "C:/Users/jstark/Projects/sampleData/AMC_2011-11/Ex365Em505T12000ms.nii";
    if (argc != 2 ) {
        std::cerr << "Usage " << argv[0] << " filename" <<std::endl;
        return 1;
    }
    const std::string inputFilename = argv[1]; //"C:/Users/jstark/Projects/sampleData/AMC_2011-11/pig in ii.nii";

    MainWindow mw;

    mw.show();

    return theApp.exec();

}
