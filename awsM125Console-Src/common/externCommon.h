#ifndef EXTERNCOMMON
#define EXTERNCOMMON

extern void clientConnect(int *sock, int port, const char *ipaddr, const char *NameOfThread);
extern void CloseSocket(int socket);

extern void setCaption(QWidget *widget, int widgetType, int lngInd, QString textR, QString textE, QString textP);

#endif // EXTERNCOMMON

