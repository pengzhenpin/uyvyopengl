#include "Player.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <vector>
#include <QFile>
#include <QOpenGLTexture>
class PlayerRenderer : public QQuickFramebufferObject::Renderer {
public:
    PlayerRenderer(QColor c = QColor(0x33, 0x33, 0x33));
    // Renderer interface
protected:
    void render();
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
    void filt();

private:
    QSize size;
    QColor color;
    QOpenGLFunctions_4_2_Core *func = nullptr;
    QOpenGLShaderProgram program;
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;
    int width = 1920;
    int height = 1080;
    QByteArray data;
    QOpenGLTexture *texture = nullptr;
};

Player::Player() : color(0x33, 0x33, 0x33)
{
}

Player::~Player()
{
}

QQuickFramebufferObject::Renderer *Player::createRenderer() const
{
    renderer = new PlayerRenderer(color);
    return renderer;
}

PlayerRenderer::PlayerRenderer(QColor c) : color(c), data(1920 * 2 * 1080, 0)
{
    func = new QOpenGLFunctions_4_2_Core();
    func->initializeOpenGLFunctions();
    func->glClearColor(color.redF(), color.greenF(), color.blueF(), 1.0f);
    bool success = program.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertex.vert");
    if (!success)
        qInfo() << "addCacheableShaderFromSourceCode failed!" << program.log();
    success = program.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragment.frag");
    if (!success)
        qInfo() << "addCacheableShaderFromSourceCode failed!" << program.log();

    if (!program.link())
        qInfo() << program.log();

    texCoords << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(0.0f, 1.0f) << QVector2D(1.0f, 1.0f);
    QFile file(":/1920x1080.UYVY");
    if (file.open(QFile::ReadOnly)) {
        data = file.readAll();
        file.close();
    }
}

void PlayerRenderer::render()
{
    if (!texture) {
        texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        texture->setWrapMode(QOpenGLTexture::Repeat);
        texture->setSize(width / 2, height);
        texture->setFormat(QOpenGLTexture::TextureFormat::RGBAFormat);
        texture->allocateStorage();
        texture->setData(QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt8, data.data());
    }
    filt();
    func->glDepthMask(true);
    func->glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    func->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    func->glFrontFace(GL_CW);
    func->glCullFace(GL_FRONT);
    func->glEnable(GL_CULL_FACE);
    func->glEnable(GL_DEPTH_TEST);

    program.bind();
    int location = 0;
    program.enableAttributeArray(location);
    program.enableAttributeArray(location + 1);
    program.setAttributeArray(location, vertices.constData());
    program.setAttributeArray(location + 1, texCoords.constData());
    texture->bind();
    func->glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
    texture->release();

    program.disableAttributeArray(location);
    program.disableAttributeArray(location + 1);
    program.release();
}

QOpenGLFramebufferObject *PlayerRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    this->size = size;
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

void PlayerRenderer::filt()
{
    if (size.width() <= 0 || size.height() <= 0 || width <= 0 || height <= 0)
        return;
    int vw = size.width() / 2 * 2;   // view width
    int vh = size.height() / 2 * 2;  // view height
    QSize vsize = QSize(width, height / 2 * 2);
    vsize.scale(vw, vh, Qt::KeepAspectRatio);
    float w = vsize.width() / 2 * 2;
    float h = vsize.height() / 2 * 2;
    vertices.clear();
    vertices << QVector3D(-w / vw, -h / vh, 0.0f) << QVector3D(w / vw, -h / vh, 0.0f)
             << QVector3D(-w / vw, h / vh, 0.0f) << QVector3D(w / vw, h / vh, 0.0f);
}
