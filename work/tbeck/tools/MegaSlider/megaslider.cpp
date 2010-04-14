#include "megaslider.h"
#include "ui_megaslider.h"

using namespace std;

MegaSlider::MegaSlider( QWidget *parent ) :
QFrame( parent ), m_ui( new Ui::MegaSlider ){
    m_ui->setupUi(this);
    m_ui->slider->setTracking( true );
    m_ui->slider->setMinimum( 0 );
    m_ui->slider->setSingleStep( 1 );
    m_ui->slider->setPageStep( 10 );
    _minimum = 0;
    _maximum = 10;
    _step = 1;
    _value = 0;
    _tracking = false;
    _alignment = Qt::AlignHCenter;
    adjustRange();
    adjustElements();
    m_ui->sliderLabel->setAlignment( _alignment );
    this->connect( m_ui->slider, SIGNAL(valueChanged(int)), SLOT(sliderMoved(int)) );
    this->connect( m_ui->slider, SIGNAL(sliderReleased()), SLOT(sliderReleased()));
    this->connect( m_ui->rightButton, SIGNAL(clicked()), SLOT(incrementValue()) );
    this->connect( m_ui->maxButton, SIGNAL(clicked()), SLOT(maximizeValue()) );
    this->connect( m_ui->leftButton, SIGNAL(clicked()), SLOT(decrementValue()) );
    this->connect( m_ui->minButton, SIGNAL(clicked()), SLOT(minimizeValue()) );
}

MegaSlider::~MegaSlider(){
    delete m_ui;
}

void MegaSlider::adjustRange(){
    m_ui->slider->setMaximum( (int)( ( _maximum - _minimum ) / _step ) );   ///////////////////////////
}

double MegaSlider::value(){
    return _value;
}

void MegaSlider::setValue( double val ){
    if( val > _maximum )
        _value = _maximum;
    else if( val < _minimum )
        _value = _minimum;
    else
        _value = val;
    adjustElements();
}

int MegaSlider::intValue(){
    return (int)_value;
}

void MegaSlider::incrementValue(){
    setValue( _value + _step );
    emit( valueChanged(_value) );
}

void MegaSlider::decrementValue(){
    setValue( _value - _step );
    emit( valueChanged(_value) );
}

void MegaSlider::maximizeValue(){
    setValue( _maximum );
    emit( valueChanged(_value) );
}

void MegaSlider::minimizeValue(){
    setValue( _minimum );
    emit( valueChanged(_value) );
}

double MegaSlider::minimum(){
    return _minimum;
}

void MegaSlider::setMinimum( double val ){
    _minimum = val;
    if( _minimum > _maximum )
        _maximum = _minimum;
    if( _value < _minimum )
        _value = _minimum;
    adjustRange();
}

double MegaSlider::maximum(){
    return _maximum;
}

void MegaSlider::setMaximum( double val ){
    _maximum = val;
    if( _maximum < _minimum )
        _minimum = _maximum;
    if( _value > _maximum )
        _value = _maximum;
    adjustRange();
}

double MegaSlider::step(){
    return _step;
}

void MegaSlider::setStep( double val ){
    _step = min( _maximum - _minimum, val );
    adjustRange();
}

QString MegaSlider::label(){
    return _label;
}

void MegaSlider::setLabel( QString lbl ){
    _label = lbl;
    m_ui->sliderLabel->setText( _label );
}

bool MegaSlider::tracking(){
    return _tracking;
}

void MegaSlider::setTracking( bool trk ){
    _tracking = trk;
}

Qt::Alignment MegaSlider::alignment(){
    return _alignment;
}

void MegaSlider::setAlignment( Qt::Alignment aln ){
    _alignment = aln;
    m_ui->sliderLabel->setAlignment( _alignment );
}

void MegaSlider::adjustElements(){
    m_ui->number->setText( QString::number( _value ) );
    int b = 10;
    int l = m_ui->slider->geometry().left();
    int w = m_ui->slider->geometry().width();
    int a = m_ui->number->geometry().width();
    l += (int)( ( _value - _minimum ) * ( w - b ) / ( _maximum - _minimum ) - a/2 - b/2 );
    m_ui->number->move( l, 0 );
    if( !blockSlider ){
        m_ui->slider->blockSignals( true );
        m_ui->slider->setValue( (int)( ( _value - _minimum) / _step ) );
        m_ui->slider->blockSignals( false );
    }
}

void MegaSlider::sliderReleased(){
    convertSlider( true );
}

void MegaSlider::sliderMoved( int val ){
    convertSlider( _tracking || !m_ui->slider->isSliderDown() );
}

void MegaSlider::convertSlider( bool emitSignal ){
    blockSlider = true;
    setValue( m_ui->slider->value() * _step + _minimum );
    blockSlider = false;
    if( emitSignal )
        emit( valueChanged(_value) );
}


void MegaSlider::resizeEvent( QResizeEvent* e ){
    QFrame::resizeEvent( e );
    adjustElements();
}
