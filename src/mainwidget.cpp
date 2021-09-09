#include "mainwidget.h"
#include "Camera.hpp"

#include <QMouseEvent>

#include <cmath>

MainWidget::~MainWidget()
{
    makeCurrent();
    delete _texture1;
    delete _texture2;
    doneCurrent();

    _vbo.destroy();
    _vao.destroy();
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::MiddleButton) {
        _camera.startRotateEvent({float(e->pos().y()), -float(e->pos().x()), 0});
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::MiddleButton) {
        _camera.stopRotateEvent({float(e->pos().y()), -float(e->pos().x()), 0});
    }
}

//{float(e->pos().x()), float(e->pos().y()), 0}

void MainWidget::mouseMoveEvent(QMouseEvent *e)
{
    _camera.rotateEvent({float(e->pos().y()), -float(e->pos().x()), 0});
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W: _camera.startMoveEvent({0, 0, 1}); return;
    case Qt::Key_S: _camera.startMoveEvent({0, 0, -1}); return;
    case Qt::Key_A: _camera.startMoveEvent({1, 0, 0}); return;
    case Qt::Key_D: _camera.startMoveEvent({-1, 0, 0}); return;
    }
    
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W: _camera.stopMoveEvent({0, 0, 1}); return;
    case Qt::Key_S: _camera.stopMoveEvent({0, 0, -1}); return;
    case Qt::Key_A: _camera.stopMoveEvent({1, 0, 0}); return;
    case Qt::Key_D: _camera.stopMoveEvent({-1, 0, 0}); return;
    }
}

void MainWidget::timerEvent(QTimerEvent *)
{
    update();
}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Use QBasicTimer because its faster than QTimer
    _timer.start(12, this);

    _vbo.create();
    _vao.create();
}

void MainWidget::initShaders()
{
    if (!_program.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/res/vshader.glsl"))
        close();

    if (!_program.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/res/fshader.glsl"))
        close();

    if (!_program.link())
        close();

    if (!_program.bind())
        close();
}

void MainWidget::initTextures()
{
    _texture1 = new QOpenGLTexture(QImage(":/res/container.jpg").mirrored());
    _texture1->setMinificationFilter(QOpenGLTexture::Nearest);
    _texture1->setMagnificationFilter(QOpenGLTexture::Linear);
    _texture1->setWrapMode(QOpenGLTexture::Repeat);

    _texture2 = new QOpenGLTexture(QImage(":/res/awesomeface.png").mirrored());
    _texture2->setMinificationFilter(QOpenGLTexture::Nearest);
    _texture2->setMagnificationFilter(QOpenGLTexture::Linear);
    _texture2->setWrapMode(QOpenGLTexture::Repeat);
}

void MainWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MainWidget::paintGL()
{
    _camera.update();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model;
    model.rotate(45, 0.5f, 1.0f, 0.0f);

    QMatrix4x4 view = _camera.lookAt();
    //qDebug() << view;

    QMatrix4x4 projection;
    projection.perspective(45.0f, width() / height(), 0.1f, 100.0f);

    GLfloat vertices[] = {
        // Позиции          // Цвета             // Текстурные координаты
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Нижний левый
        0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
        0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Верхний левый

        0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Нижний левый
        0.5f, 0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
        0.5f, 0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
        0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Верхний левый

        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Нижний левый
        0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
        0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
        -0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Верхний левый

        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Нижний левый
        0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
        0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Верхний левый

        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Нижний левый
        -0.5f, 0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
        -0.5f, 0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Верхний левый

        -0.5f, 0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Нижний левый
        0.5f, 0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
        0.5f, 0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
        -0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Верхний левый
    };

    _vbo.bind();
    _vbo.allocate(vertices, sizeof(vertices));
    _vao.bind();

    {
        auto attr = _program.attributeLocation("position");
        if (attr >= 0) {
            _program.enableAttributeArray(attr);
            glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(sizeof(GLfloat) * 0));
        }
    }

    {
        auto attr = _program.attributeLocation("color");
        if (attr >= 0) {
            _program.enableAttributeArray(attr);
            glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(sizeof(GLfloat) * 3));
        }
    }

    {
        auto attr = _program.attributeLocation("texCoord");
        if (attr >= 0) {
            _program.enableAttributeArray(attr);
            glVertexAttribPointer(attr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(sizeof(GLfloat) * 6));
        }
    }

    _vao.release();
    _vbo.release();

    _program.bind();
    _program.setUniformValue("model", model);
    _program.setUniformValue("view", view);
    _program.setUniformValue("projection", projection);
    _program.setUniformValue(_program.uniformLocation("ourTexture1"), 0);
    _program.setUniformValue(_program.uniformLocation("ourTexture2"), 1);
    _texture1->bind(0);
    _texture2->bind(1);

    _vbo.bind();
    _vao.bind();

    glCullFace(GL_BACK);
    glDrawArrays(GL_QUADS, 0, 12);
    glCullFace(GL_FRONT);
    glDrawArrays(GL_QUADS, 12, 12);

    _vbo.release();
    _vao.release();

    _texture1->release(0);
    _texture2->release(1);
    _program.release();
}
