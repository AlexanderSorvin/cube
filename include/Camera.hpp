#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include <QPoint>

class Camera
{
public:
    Camera() {
        camera.translate({0, 0, -3});
    }

    QMatrix4x4 lookAt() const
    {
        auto position = camera.map({0, 0, 0});
        auto target = camera.map(-front);
        auto up = camera.map({0, 1, 0}) - position;

        QMatrix4x4 M;
        M.lookAt(position, target, up);
        return M;
    }

    void startMoveEvent(const QVector3D &delta) 
    {
        _m.changeDelta(delta);
    }

    void stopMoveEvent(const QVector3D &delta)
    {
        _m.changeDelta(-delta);
    }

    void startRotateEvent(const QVector3D &v)
    {
        _r.on(v);
    }

    void stopRotateEvent(const QVector3D &v)
    {
        _r.off(v);
    }

    void rotateEvent(const QVector3D &v)
    {
        _r.changeDelta(v);
    }

    void update() 
    {
        static auto lastFrame = std::chrono::steady_clock::now();

        auto currentFrame = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrame - lastFrame).count() / 1000.0;
        camera.translate(_m.delta() * deltaTime);
        auto q = _r.delta() * deltaTime;
        if (!q.isNull()) {
            camera.translate(-front);
            camera.rotate(q);
            camera.translate(front);
        }
        lastFrame = currentFrame;
    }

protected:
    QMatrix4x4 camera;

    struct Move 
    {
        float speed = 1;

        QVector3D delta() const 
        {
            return _delta.normalized() * speed;
        }

        void changeDelta(const QVector3D &v) 
        {
            _delta += v;
        }
    protected:
        QVector3D _delta{0, 0, 0};
    } _m;

    struct Rotate 
    {
        float w = 10;

        QQuaternion delta() 
        {
            auto r = _delta;
            _delta = QVector3D(0, 0, 0);
            return QQuaternion(r.length() * w, r.normalized());
        }

        void changeDelta(const QVector3D &v) 
        {
            if (active) {
                _delta += v - prevMousePos;
            }
            prevMousePos = v;
        }

        void on(const QVector3D &v) 
        {
            active = true;
            prevMousePos = v;
        }

        void off(const QVector3D &v)
        {
            active = false;
            _delta += v - prevMousePos;
            prevMousePos = v;
        }

    protected:
        QVector3D prevMousePos;
        bool active = false;
        QVector3D _delta{0, 0, 0};
    } _r;

    static constexpr QVector3D front{0, 0, -1};
};


