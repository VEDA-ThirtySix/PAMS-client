#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QByteArray>

class TcpManager : public QObject {
    Q_OBJECT

public:
    // Singleton 인스턴스를 반환하는 메서드
    static TcpManager& instance() {
        static TcpManager instance; // 정적 인스턴스
        return instance;
    }

    void connectToServer(const QString& host, quint16 port);
    bool parse_header();

private:
    explicit TcpManager(QObject *parent = nullptr); // private 생성자
    ~TcpManager();
    TcpManager(const TcpManager&) = delete; // 복사 방지
    TcpManager& operator=(const TcpManager&) = delete; // 대입 방지

    QTcpSocket *tcpSocket;
    QString m_serverAddress;
    quint16 m_serverPort;
    QByteArray buffer;
    quint64 contentLength;
    bool headerParsed;

private slots:
    void on_readyRead();
    void handleDisconnect();
    void setupSocket();

signals:
    void plateDataReceived(const QByteArray &data);
};

#endif // TCPMANAGER_H
