#include "mainwidget.h"

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
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{

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
}

void MainWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    GLfloat vertices[] = {
        // Позиции          // Цвета             // Текстурные координаты
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Нижний левый
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Верхний левый
    };

    _vbo.bind();
    _vbo.allocate(vertices, sizeof(vertices));
    _vao.bind();

    {
        auto attr = _program.attributeLocation("position");
        if (attr >= 0) {
            _program.enableAttributeArray(attr);
            glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(sizeof(GLfloat) * 0));
            qDebug() << 1 << attr << glGetError();
        }
    }

    {
        auto attr = _program.attributeLocation("color");
        if (attr >= 0) {
            _program.enableAttributeArray(attr);
            glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(sizeof(GLfloat) * 3));
            qDebug() << 2 << attr << glGetError();
        }
    }

    {
        auto attr = _program.attributeLocation("texCoord");
        if (attr >= 0) {
            _program.enableAttributeArray(attr);
            glVertexAttribPointer(attr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(sizeof(GLfloat) * 6));
            qDebug() << 3 << attr << glGetError();
        }
    }

    _vao.release();
    _vbo.release();

    _program.bind();
    //glUniform2fv(program->uniformLocation("uBlur"), KERNEL_QUARTER, blur1);
    _program.setUniformValue(_program.uniformLocation("ourTexture1"), 0);
    _program.setUniformValue(_program.uniformLocation("ourTexture2"), 1);
    qDebug() << 4 << glGetError();
    _texture1->bind(0);
    _texture2->bind(1);

    _vbo.bind();
    _vao.bind();
    //glViewport(0, 0, outputTex->width, outputTex->height);

    //glDisable(GL_BLEND);
    glDrawArrays(GL_QUADS, 0, 4);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_QUADS, 4, GL_FLOAT, 0);
    qDebug() << 10 << glGetError();

    _vbo.release();
    _vao.release();

    _texture1->release(0);
    _texture2->release(1);
    _program.release();
}
