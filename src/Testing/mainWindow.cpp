#include "mainWindow.h"

#include "highResPipeline.h"
#include "lowResPipeline.h"
#include "imageSlicePipeline.h"

#include <stdexcept>

#include <QtCore>
#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QVBoxLayout>

const double IsoValue = 127; // Isosurface value
const double WindowValue = 256;    // Window / level for image slices.
const double LevelValue = 127.5;
const double DownSamplingValue = 0.25; // down sampling on each axis.

void printBounds( const double * bounds )
{
    for ( int i(0); i<6; ++i) {
        if ( i != 0 )
            std::cout << " , ";
        std::cout << bounds[i];
    }
}
void printExtents( const int * extents )
{
    for ( int i(0); i<6; ++i) {
        if ( i != 0 )
            std::cout << " , ";
        std::cout << extents[i];
    }
}


// Callback for the interaction
// For the timers the vtkRenderWindowInteractor returned by the QVTK widget appears to not support timers.
class vtkBWCallback2 : public vtkCommand
{
public:
    static vtkBWCallback2 *New() 
    { return new vtkBWCallback2; }

    virtual void Execute(vtkObject *caller, unsigned long evId, void*)
    {
        if ( evId != vtkCommand::TimerEvent) {
            vtkBoxWidget2 *boxWidget = vtkBoxWidget2::SafeDownCast(caller);

            vtkBoxRepresentation *boxRep = vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation());

            //boxRep->GetTransform(this->Transform);

            //this->ClipBox->SetTransform(this->Transform);

            double bounds[6];
            const double * brBounds = boxRep->GetBounds();
            std::copy( brBounds, brBounds+6, bounds);
            {
                //printBounds(bounds);   std::cout << std::endl;
                double origin[3], spacing[3];
                int extent[6];
                vtkImageData * id = vtkImageData::SafeDownCast( this->ExtractVOI->GetInput());
                id->UpdateInformation();
                id->GetSpacing(spacing);
                id->GetOrigin(origin);
                id->GetWholeExtent(extent);
                int range[6];
                for ( int i(0); i<3; ++i) {
                    range[2*i] = std::floor( (bounds[2*i] - origin[i]) / spacing[i] );
                    range[2*i + 1] = std::ceil( (bounds[2*i + 1] - origin[i]) / spacing[i] );

                    range[2*i] = std::max( range[2*i], extent[2*i] );
                    range[2*i + 1] = std::min( range[2*i + 1], extent[2*i + 1] );
                }

                std::copy( range, range+6, LatestExtent);
                std::copy( bounds, bounds+6, LatestBounds);
            }

            double centreBound[3];
            for ( int i(0); i<3; ++i) {
                centreBound[i] = bounds[2*i] + ( ( bounds[2*i + 1] - bounds[2*i]) *0.5 );
            }

            this->AxisActorXY->SetFullExtent( LatestExtent );
            this->AxisActorXZ->SetFullExtent( LatestExtent );
            this->AxisActorYZ->SetFullExtent( LatestExtent );

            //printExtents(extent);   std::cout << std::endl;
            //printExtents(range);   std::cout << std::endl;

            boxWidget->GetInteractor()->Render();

            if ( m_MainWindow )
                m_MainWindow->restartRefreshTimer();
            else {
                this->ExtractVOI->SetVOI(LatestExtent);
                this->ClipBox->SetBounds(LatestBounds);
            }
        } else {

            std::cout << "Current slice extent : iMin,iMax, jMin,jMax, kMin,kMax"<<std::endl;
            printExtents(LatestExtent); std::cout << std::endl;

            this->ExtractVOI->SetVOI(LatestExtent);
            this->ClipBox->SetBounds(LatestBounds);
        }
    }

    vtkBWCallback2()  : m_MainWindow(NULL) {
        this->Transform = vtkSmartPointer<vtkTransform>::New();
    }

    ~vtkBWCallback2() {
    }

    void SetClipBox( vtkBox * box ) { this->ClipBox = box; }
    void SetVOIExtractor( vtkExtractVOI * Extractor) { this->ExtractVOI = Extractor; }
    void SetMainWindow( MainWindow * mw) {
        this->m_MainWindow = mw;
    }

    void SetSliceActors( ImageSlicePipeline * xy, ImageSlicePipeline * xz, ImageSlicePipeline * yz ){
        this->AxisActorXY = xy;
        this->AxisActorXZ = xz;
        this->AxisActorYZ = yz;
    }

protected:
    vtkSmartPointer<vtkBox> ClipBox;
    vtkSmartPointer<vtkTransform> Transform;
    vtkSmartPointer<vtkExtractVOI> ExtractVOI;

    vtkSmartPointer<ImageSlicePipeline> AxisActorXY;
    vtkSmartPointer<ImageSlicePipeline> AxisActorXZ;
    vtkSmartPointer<ImageSlicePipeline> AxisActorYZ;
    
    MainWindow * m_MainWindow;
    int LatestExtent[6];
    double LatestBounds[6];
    int TimerId;
};

vtkSmartPointer<vtkBoxWidget2> addBoxWidget(const double * bounds, vtkRenderWindowInteractor * iren, vtkBWCallback2 * myCallback )
{
    double boundsLocal[6];
    std::copy( bounds, bounds+6, boundsLocal);
    vtkSmartPointer<vtkBoxRepresentation> boxRep = vtkSmartPointer<vtkBoxRepresentation>::New();
    boxRep->SetPlaceFactor( 0.2 );
    boxRep->PlaceWidget(boundsLocal);

    vtkSmartPointer<vtkBoxWidget2> boxWidget = vtkSmartPointer<vtkBoxWidget2>::New();
    boxWidget->SetInteractor( iren );
    boxWidget->SetRepresentation( boxRep );
    boxWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
    boxWidget->SetPriority(1);
    boxWidget->RotationEnabledOff();

    return boxWidget;
}


class MainWindow::MainWindowPrivate {
public:
    QVTKWidget * vtkWidget;
    QVTKWidget * vtkWidgetXY;
    QVTKWidget * vtkWidgetXZ;
    QVTKWidget * vtkWidgetYZ;

    vtkSmartPointer<vtkBox> clipBox;

    vtkSmartPointer<LowResPipeline> lowRes;

    vtkSmartPointer<HighResPipeline> highRes;

    vtkSmartPointer<ImageSlicePipeline> SliceXY;
    vtkSmartPointer<ImageSlicePipeline> SliceYZ;
    vtkSmartPointer<ImageSlicePipeline> SliceXZ;

    vtkSmartPointer<vtkBWCallback2> myCallback;
    vtkSmartPointer<vtkBoxWidget2> boxWidget;

    std::string inputFilename;

    QTimer *refreshBoxTimer;
 };

MainWindow::MainWindow()
    : d(new MainWindowPrivate)
{
    QWidget * cWidget = new QWidget(this);
    this->setCentralWidget(cWidget);

    QHBoxLayout * hlayout = new QHBoxLayout;
    cWidget->setLayout( hlayout );
    
    QVBoxLayout * vlayout = new QVBoxLayout;
    hlayout->addLayout( vlayout );

    d->vtkWidget = new QVTKWidget(this);
    hlayout->addWidget(d->vtkWidget);

    d->vtkWidgetXY = new QVTKWidget(this);
    d->vtkWidgetXZ = new QVTKWidget(this);
    d->vtkWidgetYZ = new QVTKWidget(this);

    vlayout->addWidget( d->vtkWidgetXY );
    vlayout->addWidget( d->vtkWidgetXZ );
    vlayout->addWidget( d->vtkWidgetYZ );

    this->createMenus();

    d->refreshBoxTimer = new QTimer(this);
    d->refreshBoxTimer->setSingleShot(true);

    connect(d->refreshBoxTimer, SIGNAL(timeout()), this, SLOT(onRefreshTimer()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenus()
{
    QAction * openAct = new QAction( tr("&Open..."), this);
    //d->openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    QMenu * fileMenu = this->menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
}

void MainWindow::BuildPipeLine()
{
    d->clipBox = vtkSmartPointer<vtkBox>::New();

    d->lowRes = vtkSmartPointer<LowResPipeline>::New();
    d->lowRes->SetIsoValue(IsoValue);
    d->lowRes->BuildPipeLine(d->inputFilename, d->clipBox, DownSamplingValue);

    d->highRes = vtkSmartPointer<HighResPipeline>::New();
    d->highRes->SetIsoValue(IsoValue);
    d->highRes->BuildPipeLine(d->inputFilename);

    d->SliceXY = vtkSmartPointer<ImageSlicePipeline>::New();
    d->SliceYZ = vtkSmartPointer<ImageSlicePipeline>::New();
    d->SliceXZ = vtkSmartPointer<ImageSlicePipeline>::New();

    d->SliceXY->SetWindowLevel(WindowValue,LevelValue);
    d->SliceXZ->SetWindowLevel(WindowValue,LevelValue);
    d->SliceYZ->SetWindowLevel(WindowValue,LevelValue);

    d->SliceXY->BuildPipeline(d->inputFilename, ImageSlicePipeline::AXIS::XY, d->vtkWidgetXY->GetRenderWindow());
    d->SliceXZ->BuildPipeline(d->inputFilename, ImageSlicePipeline::AXIS::XZ, d->vtkWidgetXZ->GetRenderWindow());
    d->SliceYZ->BuildPipeline(d->inputFilename, ImageSlicePipeline::AXIS::YZ, d->vtkWidgetYZ->GetRenderWindow());

    d->myCallback = vtkSmartPointer<vtkBWCallback2>::New();
    d->myCallback->SetSliceActors(d->SliceXY, d->SliceXZ, d->SliceYZ);
    d->myCallback->SetClipBox(d->clipBox);
    d->myCallback->SetVOIExtractor(d->highRes->GetExtractVOI());

    // Renderer and window
    vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();

    ren->AddActor(d->lowRes->getActor());

    double bounds[6];
    d->lowRes->GetImageBounds(bounds);
    if ( bounds[1] - bounds[0] <= 0 ) {
        throw( std::runtime_error("Bad bounds") );
    }

    vtkRenderWindowInteractor * rWinIn = d->vtkWidget->GetInteractor();
    //printBounds(bounds);   std::cout << std::endl;
    d->boxWidget = addBoxWidget(bounds, rWinIn, d->myCallback);

    d->vtkWidget->GetRenderWindow()->AddRenderer(ren);
    vtkInteractorStyleSwitch * sw = vtkInteractorStyleSwitch::SafeDownCast( rWinIn->GetInteractorStyle() );
    if ( sw ) 
        sw->SetCurrentStyleToTrackballCamera();

    d->boxWidget->EnabledOn();

    //Force initial update
    d->myCallback->Execute(d->boxWidget,vtkCommand::InteractionEvent,0);

    // To prevent reading all the data, the actor is only added after we have initial extent.
    ren->AddActor(d->highRes->getActor());

    d->vtkWidget->GetRenderWindow()->Render();
    d->myCallback->SetMainWindow(this);
    vtkBoxRepresentation *boxRep = vtkBoxRepresentation::SafeDownCast(d->boxWidget->GetRepresentation());
    d->clipBox->SetBounds(boxRep->GetBounds());

    ren->ResetCamera();
}

void MainWindow::setFilename( const QString & fn )
{
    d->inputFilename = fn.toAscii().data();
    this->BuildPipeLine();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.mha *.mhd *.nii)"));
    if ( fileName.isNull() )
        return;

    this->setFilename(fileName);
}

void MainWindow::onRefreshTimer()
{
    d->myCallback->Execute( 0, vtkCommand::TimerEvent, 0);
    d->vtkWidget->GetInteractor()->Render();
}

void MainWindow::restartRefreshTimer()
{
    d->refreshBoxTimer->start(500); // msec
}
