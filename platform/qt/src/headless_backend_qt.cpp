#include <mbgl/gl/headless_backend.hpp>

#include <QGLWidget>

#if QT_VERSION >= 0x050000
#include <QOpenGLContext>
#else
#include <QGLContext>
#endif

#include <cassert>

namespace mbgl {

class QtImpl : public HeadlessBackend::Impl {
public:
    ~QtImpl() final = default;

    void activateContext() final {
        widget.makeCurrent();
    }

    void deactivateContext() final {
        widget.doneCurrent();
    }

private:
    QGLWidget widget;
};

gl::ProcAddress HeadlessBackend::initializeExtension(const char* name) {
#if QT_VERSION >= 0x050000
        QOpenGLContext* thisContext = QOpenGLContext::currentContext();
        return thisContext->getProcAddress(name);
#else
        const QGLContext* thisContext = QGLContext::currentContext();
        return reinterpret_cast<gl::ProcAddress>(thisContext->getProcAddress(name));
#endif
}

void HeadlessBackend::createContext() {
    assert(!hasContext());
    impl = std::make_unique<QtImpl>();
}

} // namespace mbgl
