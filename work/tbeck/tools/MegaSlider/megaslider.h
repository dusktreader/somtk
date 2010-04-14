#pragma once

#include <QtGui/QFrame>

namespace Ui {
    class MegaSlider;
}

class MegaSlider : public QFrame {
    Q_OBJECT
    Q_PROPERTY( double maximum READ maximum WRITE setMaximum )
    Q_PROPERTY( double minimum READ minimum WRITE setMinimum )
    Q_PROPERTY( double value READ value WRITE setValue )
    Q_PROPERTY( double step READ step WRITE setStep )
    Q_PROPERTY( bool tracking READ tracking WRITE setTracking )
    Q_PROPERTY( Qt::Alignment alignment READ alignment WRITE setAlignment )
    Q_PROPERTY( QString label READ label WRITE setLabel )
public:
    MegaSlider( QWidget* parent=0 );
    ~MegaSlider();

    double value();
    void   setValue( double val );
    int    intValue();

    double minimum();
    void   setMinimum( double val );

    double maximum();
    void   setMaximum( double val );

    double step();
    void   setStep( double val );

    bool   tracking();
    void   setTracking( bool trk );

    Qt::Alignment alignment();
    void          setAlignment( Qt::Alignment align );

    QString label();
    void    setLabel( QString lbl );

signals:
    void valueChanged( double value );

private slots:
    void sliderMoved( int value );
    void sliderReleased();
    void incrementValue();
    void decrementValue();
    void maximizeValue();
    void minimizeValue();

protected:
    QString       _label;
    double        _minimum;
    double        _maximum;
    double        _step;
    double        _value;
    bool          _tracking;
    Qt::Alignment _alignment;

    bool blockSlider;
    void convertSlider( bool emitSignal );
    void resizeEvent( QResizeEvent *e );
    void adjustRange();
    void adjustElements();

private:
    Ui::MegaSlider *m_ui;
};
