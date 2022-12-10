#ifndef PLAYER_H
#define PLAYER_H

#include <QQuickFramebufferObject>

class PlayerRenderer;
class Player : public QQuickFramebufferObject {
    Q_OBJECT
public:
    Player();
    ~Player();
    Renderer *createRenderer() const;

private:
    mutable PlayerRenderer *renderer = nullptr;
    QColor color;
};

#endif  // PLAYER_H
