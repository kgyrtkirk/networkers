// // #include	"detectable.h"
// // 
// // void	NetWorker::send_message(int type,	Detectable*dst)
// // {
// // 	//	this will fill out the header
// // 	Message	*m=new Message(type,	this,	dst);
// // 	switch(type)
// // 	{
// // 		SCAN_INFO:
// // 			m->si.query_pos[0]=dst.pos[0];
// // 			m->si.query_pos[1]=dst.pos[1];
// // 	}
// // 	dst->msg_queue.add(m);
// // 	
// // }
