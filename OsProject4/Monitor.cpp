/*Monitor file*/


//Public section

//Initalize Monitor
Monitor::Monitor(){
  pthread_cond_init(&OktoRead,NULL);
  pthread_cond_init(&OktoWrite,NULL);
  pthread_mutex_init(&mLock, NULL);
  pthread_mutex_init(&outFileLock, NULL);

  char *fullfilepath = (char*) malloc(30);
  char *thisprojectfile_out = (char*) malloc(13);
  strcpy(thisprojectfile_out,"project4_out.txt");
  strcpy(fullfilepath,filebase);
  strcat(fullfilepath, thisprojectfile_out);

  outfile.open(fullfilepath);

  activeRead=0;
  activeWrite=0;
  waitingRead=0;
  waitingWrite=0;
}

//Destory Monitor
Monitor::~Monitor(){
  pthread_cond_destroy(&OktoRead);
  pthread_cond_destroy(&OktoWrite);
  pthread_mutex_destroy(&mLock);
  pthread_mutex_destroy(&outFileLock);

  outfile.close();
}
// See if reading can begin
void Monitor::startReading(){
  lock();
    while((activeWrite + waitingWrite)>0){   //If writing or if waiting to write hold
     waitingRead++;
     pthread_cond_wait(&OktoRead, &mLock);
     waitingRead--;
    }
  unlock();
}

void Monitor::stopReading(){
 lock();
  activeRead--;
  if (activeRead == 0 && waitingWrite > 0){
    pthread_cond_signal(&OktoWrite);
  }
  unlock();
}

void Monitor::startWriting() {
  lock();
     while ((activeWrite + activeRead) > 0) {
        waitingWrite++;
        pthread_cond_wait(&OktoWrite, &mLock);
        waitingWrite--;
     }
  unlock();
}

void Monitor::stopWriting(){
  lock();
   activeWrite--;
    if (waitingWrite > 0) {
      pthread_cond_signal(&OktoWrite);
    }
    else if (waitingRead > 0) {
      pthread_cond_broadcast(&OktoRead);
    }
  unlock();
  }

//File IO
void Monitor::fileRead(string dB, int tID){
  fileLock();
  outfile << ">>> DB value set to: " << dB.c_str() << " by reader thread numaber: " << tID << endl;
  fileUnlock(); 
}

void Monitor::fileWrite(string dB, int tID){
  fileLock();
  outfile << ">>> DB value set to: " << dB.c_str() << " by writer thread numaber: " << tID << endl;
  fileUnlock();
}

//Private
//Functions simply lock and unlock corrosponding mutexs
void Monitor::lock(){
  pthread_mutex_lock(&mLock);
}

void Monitor::fileLock(){
  pthread_mutex_lock(&outFileLock);
}

void Monitor::unlock(){
  pthread_mutex_unlock(&mLock);
}

void Monitor::fileUnlock(){
  pthread_mutex_unlock(&outFileLock);
}

