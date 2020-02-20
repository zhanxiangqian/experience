
virtual  bool			eventFilter(QObject *obj, QEvent *event);


bool SelectWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::WindowActivate) 
	{
		activeSelecting();
		return true;
	}
	else 
	{
		// standard event processing
		return QObject::eventFilter(obj, event);
	}
	
}


//apply
	mainWidget->installEventFilter(m_selectWidget);
