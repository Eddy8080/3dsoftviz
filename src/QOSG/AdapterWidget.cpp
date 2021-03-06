#include "QOSG/AdapterWidget.h"
#include "QOSG/CoreWindow.h"

#include <leathers/push>
#include <leathers/switch-enum>

#include <easylogging++.h>

namespace QOSG {

AdapterWidget::AdapterWidget( const QGLFormat& format, QWidget* parent, const char* name, const QGLWidget* shareWidget, WindowFlags f ) :
#if QT_VERSION > 0x050000
	QOpenGLWidget( parent, f )
#elif QT_VERSION > 0x040000
	QGLWidget( format, parent, shareWidget, f )

#else
	QGLWidget( parent, shareWidget, f )
#endif
{
	_gw = new osgViewer::GraphicsWindowEmbedded( 0,0,width(),height() );
	setFocusPolicy( Qt::StrongFocus );
}

void AdapterWidget::initializeGL()
{
#if QT_VERSION > 0x050000
	this->initializeOpenGLFunctions();
#endif
	// Set up the rendering context, define display lists etc.:

	qDebug() << "OpenGL Versions Supported: " << QGLFormat::openGLVersionFlags();
	qDebug() << "Current Context:" << this->format();

	auto context = this->context();

	qDebug() << "Context valid: " << context->isValid();
	qDebug() << "Really used OpenGl: " << context->format().majorVersion() << "." << context->format().minorVersion();
	qDebug() << "Profile: " << context->format().profile();

	qDebug() << "OpenGl information: VENDOR:       " << ( const char* )glGetString( GL_VENDOR );
	qDebug() << "                    RENDERDER:    " << ( const char* )glGetString( GL_RENDERER );
	qDebug() << "                    VERSION:      " << ( const char* )glGetString( GL_VERSION );
	qDebug() << "                    GLSL VERSION: " << ( const char* )glGetString( GL_SHADING_LANGUAGE_VERSION );
	qDebug() << "endstuff\n";

}



void AdapterWidget::resizeGL( int width, int height )
{
	// FIX: for HiDPI displays
	// without this fix, OpenGL context on retina displays is small,
	// causing to display a small OpenGL context only in the lower-left corner
	// (1/4 of the desired size)
	// NOTE: on non-retina displays devicePixelRatio() is 1, on retina 2
#if QT_VERSION > 0x050000
	width *= QApplication::desktop()->devicePixelRatio();
	height *= QApplication::desktop()->devicePixelRatio();
#endif
	// end FIX

	//zmena velkosti widgetu
	_gw->getEventQueue()->windowResize( 0, 0, width, height );
	_gw->resized( 0,0,width,height );
}

void AdapterWidget::keyPressEvent( QKeyEvent* event )
{
	//odchytavanie udalosti klavesnice
	switch ( event->key() ) {
		case ( Qt::Key_Up ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_Up );
			break;
		}
		case ( Qt::Key_Down ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_Down );
			break;
		}
		case ( Qt::Key_Right ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_Right );
			break;
		}
		case ( Qt::Key_Left ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_Left );
			break;
		}
		case ( Qt::Key_PageDown ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_Page_Down );
			break;
		}
		case ( Qt::Key_PageUp ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_Page_Up );
			break;
		}
		case ( Qt::Key_Alt ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_Alt_L );
			break;
		}
		case ( Qt::Key_Control ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_Control_L );
			break;
		}
		case ( Qt::Key_Shift ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_Shift_L );
			break;
		}
		case ( Qt::Key_O ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_O );
			break;
		}
		case ( Qt::Key_P ): {
			_gw->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KEY_P );
			break;
		}
		default: {
			_gw->getEventQueue()->keyPress( static_cast<osgGA::GUIEventAdapter::KeySymbol>( *( event->text().TOASCII().data() ) ) );
			break;
		}
	}
}

void AdapterWidget::keyReleaseEvent( QKeyEvent* event )
{
	//odchytavanie udalosti klavesnice
	switch ( event->key() ) {
		case ( Qt::Key_Up ): {
			_gw->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KEY_Up );
			break;
		}
		case ( Qt::Key_Down ): {
			_gw->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KEY_Down );
			break;
		}
		case ( Qt::Key_Right ): {
			_gw->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KEY_Right );
			break;
		}
		case ( Qt::Key_Left ): {
			_gw->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KEY_Left );
			break;
		}
		case ( Qt::Key_PageDown ): {
			_gw->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KEY_Page_Down );
			break;
		}
		case ( Qt::Key_PageUp ): {
			_gw->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KEY_Page_Up );
			break;
		}
		case ( Qt::Key_Alt ): {
			_gw->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KEY_Alt_L );
			break;
		}
		case ( Qt::Key_Control ): {
			_gw->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KEY_Control_L );
			break;
		}
		case ( Qt::Key_Shift ): {
			_gw->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KEY_Shift_L );
			break;
		}
		case ( Qt::Key_AsciiTilde ): {
			CoreWindow* coreWindow = dynamic_cast<CoreWindow*>( parent() );
			if ( coreWindow ) {
				coreWindow->addSQLInput();
			}
			break;
		}
		default: {
			_gw->getEventQueue()->keyRelease( static_cast<osgGA::GUIEventAdapter::KeySymbol>( *( event->text().TOASCII().data() ) ) );
			break;
		}
	}
}

void AdapterWidget::mousePressEvent( QMouseEvent* event )
{
	//odchytavanie udalosti mysi
	unsigned int button = 0;
	switch ( event->button() ) {
		case ( Qt::LeftButton ):
			button = 1;
			break;
		case ( Qt::MidButton ):
			button = 2;
			break;
		case ( Qt::RightButton ):
			button = 3;
			break;
		case ( Qt::NoButton ):
			button = 0;
			break;
		default:
			button = 0;
			break;
	}

	_gw->getEventQueue()->mouseButtonPress( static_cast<float>( event->x() ),static_cast<float>( event->y() ), button );
}

void AdapterWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
	//odchytavanie udalosti mysi
	unsigned int button = 0;
	switch ( event->button() ) {
		case ( Qt::LeftButton ):
			button = 1;
			break;
		case ( Qt::MidButton ):
			button = 2;
			break;
		case ( Qt::RightButton ):
			button = 3;
			break;
		case ( Qt::NoButton ):
			button = 0;
			break;
		default:
			button = 0;
			break;
	}

	_gw->getEventQueue()->mouseDoubleButtonPress( static_cast<float>( event->x() ),static_cast<float>( event->y() ), button );

}

void AdapterWidget::mouseReleaseEvent( QMouseEvent* event )
{
	//odchytavanie udalosti mysi
	unsigned int button = 0;
	switch ( event->button() ) {
		case ( Qt::LeftButton ):
			button = 1;
			break;
		case ( Qt::MidButton ):
			button = 2;
			break;
		case ( Qt::RightButton ):
			button = 3;
			break;
		case ( Qt::NoButton ):
			button = 0;
			break;
		default:
			button = 0;
			break;
	}
	_gw->getEventQueue()->mouseButtonRelease( static_cast<float>( event->x() ),static_cast<float>( event->y() ), button );
}

void AdapterWidget::mouseMoveEvent( QMouseEvent* event )
{

	//odchytavanie udalosti mysi
	_gw->getEventQueue()->mouseMotion( static_cast<float>( event->x() ),static_cast<float>( event->y() ) );
}

void AdapterWidget::wheelEvent( QWheelEvent* event )
{
	//odchytavanie udalosti mysi
	_gw->getEventQueue()->mouseScroll( ( event->delta()>0 ) ?
									   osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN );
}

} // namespace QOSG

#include <leathers/pop>
