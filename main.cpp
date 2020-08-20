#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <proxy.h>
#include <registrator.h>
#include <x_omodel.h>

class MembersInit{
public:
	using Proxy_ptr = std::shared_ptr<XOProxy>;
	using Registrator_ptr = std::shared_ptr<Registrator>;
	using Model_ptr = std::shared_ptr<XOModel>;
	enum class MembersType{
		Proxy,
		Registrator,
		XOmodel
	};
	MembersInit()
	{
		m_model_ptr = std::make_shared<XOModel>();
		m_proxy_ptr = std::make_shared<XOProxy>(m_model_ptr);
		m_registrator_ptr = std::make_shared<Registrator>(m_proxy_ptr);
	}
	void registrate(QQmlContext* context){
		context->setContextProperty("xo_model", m_model_ptr.get());
		context->setContextProperty("xo_proxy", m_proxy_ptr.get());
		context->setContextProperty("xo_registrator", m_registrator_ptr.get());
	}
private:
	Model_ptr m_model_ptr;
	Proxy_ptr m_proxy_ptr;
	Registrator_ptr m_registrator_ptr;
};

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	MembersInit members;
	members.registrate(engine.rootContext());
	const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);
	return app.exec();
}
