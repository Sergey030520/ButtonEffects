#include <QPaintEvent>
#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <QPixmap>
#include <iostream>
#include <QPainter>
#include <QMediaPlayer>
#include <QDir>

class EffectButton : public QPushButton{
public:
    EffectButton() = default;
    EffectButton(QWidget* parent);
    void paintEvent(QPaintEvent* e) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void keyPressEvent(QKeyEvent *e) override;

public slots:
    void setUp();
    void setDown();
private:
    QPixmap mCurrentButtonPixmap,
            mDownButtonPixmap, mUpButtonPixmap;
    bool isDown = false;
};

EffectButton::EffectButton(QWidget *parent){
    setParent(parent);
    setToolTip("Стоп");
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //path to the project
    QDir* pathProject = new QDir(QApplication::applicationDirPath());
    pathProject->cdUp();

    //initializing the button effect
    mUpButtonPixmap = QPixmap(pathProject->absolutePath().append(R"(\icons\btn_up.png)"));
    mDownButtonPixmap = QPixmap( pathProject->absolutePath().append(R"(\icons\btn_down.png)"));
    mCurrentButtonPixmap = mUpButtonPixmap;
    auto* effectBtn = new QMediaPlayer(parent);
    setGeometry(mCurrentButtonPixmap.rect());

    connect(this, &QPushButton::clicked, this, &EffectButton::setDown);

    connect(this, &QPushButton::clicked,  [effectBtn, pathProject](){
        effectBtn->setMedia(QUrl::fromLocalFile(
                pathProject->absolutePath().append(R"(\audio effects\audio-effect.mp3)")));
        effectBtn->setVolume(75);
        effectBtn->play();
    });
}

void EffectButton::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.drawPixmap(e->rect(), mCurrentButtonPixmap);
}

QSize EffectButton::sizeHint() const {
    return QSize(100, 100);
}

QSize EffectButton::minimumSizeHint() const {
    return sizeHint();
}

void EffectButton::keyPressEvent(QKeyEvent *e) {
    setDown();
}

void EffectButton::setDown() {
    mCurrentButtonPixmap = mDownButtonPixmap;
    update();
    QTimer::singleShot(100, this, &EffectButton::setUp);
}

void EffectButton::setUp() {
    mCurrentButtonPixmap = mUpButtonPixmap;
    update();
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    EffectButton button(nullptr);
    button.setFixedSize(100, 100);
    button.move(1000, 400);
    button.show();
    return QApplication::exec();
}
