#include <iostream>
#include <fstream>
#include <array>

using namespace std;

// loads file data to table of vectors
char fileLoad(fstream&, vector<int>[]);
// returns max value from given vector of ints
int vectorMax(vector<int>&);
// returns sum from given tab of given size
int sumTab(int[], size_t);
// performs simulation of first in first out algorithm
void fifo(vector<int>[]);
// returns how far in given vector from given position is given value (if there's not, return size of vector, if val=-1 returns size of vector + 1)
int nextRequest(vector<int>&, size_t, int&);
// performs simulation of optimal algorithm
void opt(vector<int>[]);
// returns how far before in given vector from given position is given value (if there's not, return size of vector, if val=-1 returns size of vector + 1)
int prevRequest(vector<int>&, size_t, int&);
// performs simulation of least recent used algorithm
void lru(vector<int>[]);
//
int appearCount(vector<int>&, size_t, int);
// performs simulation of most frequent used algorithm
void mfu(vector<int>[]);

fstream fOutput("../output.txt",ios::out);
int main()
{
	fstream fInput("../input.txt",ios::in);
	if(!fInput.good())
	{
		cout<<"no input file!";
		return 1;
	}

	const char size=2;
	// table of vector with pages requests and number of available frames
	// { pageID[pagesNo] : frame[0] }
	vector<int> dataArr[size];

	char fStatus=fileLoad(fInput, dataArr);
	switch(fStatus)
	{
		case 0:
			break;
		case 1:
			cout<<"not enough pages placed in file (min: 20) [input.txt:1]";
			return 1;
		case 2:
			cout<<"no information about number of frames [input.txt:2]";
			return 1;
		case 3:
			cout<<"too much frames (max: 5) [input.txt:2]";
			return 1;
//			break;
		default:
			cout<<"file data error";
			return 1;
	}

	cout<<"Choose an algorithm:\n\t1 — FIFO\n\t2 — OPT\n\t3 — LRU\n\t4 — MFU\n>> ";
	char userChoose=static_cast<char>(getchar());

	switch(userChoose)
	{
		case '1':
			fifo(dataArr);
			break;
		case '2':
			opt(dataArr);
			break;
		case '3':
			lru(dataArr);
			break;
		case '4':
			mfu(dataArr);
			break;
		default:
			cout<<"bad input.";
			return 1;
	}

	return 0;
}

char fileLoad(fstream &fInput, vector<int> tab[])
{
	//line from file
	string line;
	if(!getline(fInput, line))
		return 99;

	fInput.seekg(0);
	// number of lines in file
	char lineCounter=0;
	while(!fInput.eof()&&lineCounter<2)
	{
		getline(fInput,line);
		string temp;
		for(auto &i: line)
		{
			if(i==' '||&i==&line[line.size()-1])
			{
				if(&i==&line[line.size()-1])
					temp+=i;

				tab[lineCounter].push_back(stoi(temp));

				if(lineCounter>0)
				{
					break;
				}
				temp="";
			}
			else
			{
				temp+=i;
			}
		}
		++lineCounter;
	}


//    cout<<tab[0].size()<<"\t"<<tab[1].size()<<endl;

	// checking is there necessary number of pages
	if(tab[0].size()<20)
	{
		return 1;
	}
	// checking is there a number of frames
	if(lineCounter<2)
	{
		return 2;
	}
	// checking is number of frames low enough
	if(tab[1].size()>5)
	{
		return 3;
	}

	return 0;
}

int vectorMax(vector<int> & givenVector)
{
	int maxElementVal=0;
	for(auto &i: givenVector)
	{
		if(i>maxElementVal)
			maxElementVal=i;
	}
	return maxElementVal;
}

int sumTab(int tab[], size_t tabSize)
{
	int sum=0;
	for(size_t i=0; i<tabSize; ++i)
	{
		sum+=tab[i];
	}
	return sum;
}

void fifo(vector<int> tab[])
{
	fOutput<<"Algorithm: FIFO\n\nstates of frames:\n";
	fOutput<<"-----------------\n";
	cout<<"states of frames:\n";
	cout<<"-----------------\n";
	const size_t maxID=vectorMax(tab[0])+1;
	int errors[maxID];
	for(auto &i: errors)
	{
		i=0;
	}
	int frames[static_cast<size_t>(tab[1][0])];
	for(auto &i: frames)
	{
		i=-1;
	}
	// set to 2 to begin check with index 0;
	int lastChangedFrame=2;
	for(auto &i: tab[0])
	{
		bool isThere_=false;
		for(auto &ii: frames)
		{
			fOutput<<ii<<"\t";
			cout<<ii<<"\t";
			if(ii==i)
				isThere_=true;
		}
		fOutput<<"\n";
		cout<<"\n";
		if(!isThere_)
		{
			lastChangedFrame=lastChangedFrame==tab[1][0]-1?0:lastChangedFrame+1;
//			if(frames[lastChangedFrame]>-1)
				++errors[static_cast<size_t>(i)];
			frames[lastChangedFrame]=i;
		}
	}
	for(auto &ii: frames)
	{
		cout<<ii<<"\t";
		fOutput<<ii<<"\t";
	}

	fOutput<<"\n\nNo of page faults for every page ID:\n";
	fOutput<<"------------------------------------";
	cout<<"\n\nNo of page faults for every page ID:\n";
	cout<<"------------------------------------";
	for(int i=0; i<maxID; ++i)
	{
		fOutput<<'\n'<<i<<":\t"<<errors[i];
		cout<<'\n'<<i<<":\t"<<errors[i];
	}
	cout<<"\n\n% of page faults among all requests: "<<static_cast<double>(sumTab(errors,maxID))/(static_cast<double>(tab[0].size()))*100;
	fOutput<<"\n\n% of page faults among all requests: "<<static_cast<double>(sumTab(errors,maxID))/(static_cast<double>(tab[0].size()))*100;
}

int nextRequest(vector<int> &givenVector, size_t currentPos, int &currentVal)
{
	for(int i=static_cast<int>(currentPos+1); i<givenVector.size(); ++i)
	{
		if(givenVector[i]==currentVal)
			return i-static_cast<int>(currentPos);
	}
	if(currentVal==-1)
		return static_cast<int>(givenVector.size())+1;
	return static_cast<int>(givenVector.size());
}

void opt(vector<int> tab[])
{
	fOutput<<"Algorithm: OPT\n\nstates of frames:\n";
	fOutput<<"-----------------\n";
	cout<<"states of frames:\n";
	cout<<"-----------------\n";
	const size_t maxID=vectorMax(tab[0])+1;
	// { noOfErrors[ForThisID] }
	int errors[maxID];
	for(auto &i: errors)
	{
		i=0;
	}
	// { pageID[noOfFramesFromFile] }
	int frames[static_cast<size_t>(tab[1][0])];
	for(auto &i: frames)
	{
		i=-1;
	}
	for(auto &i: tab[0])
	{
		bool isThere_=false;
		for(auto &frame: frames)
		{
			fOutput<<frame<<"\t";
			cout<<frame<<"\t";
			if(frame==i)
				isThere_=true;
		}
		fOutput<<"\n";
		cout<<"\n";
		if(!isThere_)
		{
			// { FrameIndex : futureUsageDistance }
			int toChange[2]={0,INT_MIN};
			for(int ii=0; ii<tab[1][0]; ++ii)
			{
				int toCheck=nextRequest(tab[0],&i-&tab[0][0],frames[ii]);
				if(toCheck>toChange[1])
				{
					toChange[0]=ii;
					toChange[1]=nextRequest(tab[0],&i-&tab[0][0],frames[ii]);
				}
			}
//			if(frames[toChange[0]]>-1)
				++errors[static_cast<size_t>(i)];
			frames[toChange[0]]=i;
		}
	}
	for(auto &ii: frames)
	{
		fOutput<<ii<<"\t";
		cout<<ii<<"\t";
	}

	fOutput<<"\n\nNo of page faults for every page ID:\n";
	fOutput<<"------------------------------------";
	cout<<"\n\nNo of page faults for every page ID:\n";
	cout<<"------------------------------------";
	for(auto i=0; i<maxID; ++i)
	{
		fOutput<<'\n'<<i<<":\t"<<errors[i];
		cout<<'\n'<<i<<":\t"<<errors[i];
	}
	fOutput<<"\n\n% of page faults among all requests: "<<static_cast<double>(sumTab(errors,maxID))/(static_cast<double>(tab[0].size()))*100;
	cout<<"\n\n% of page faults among all requests: "<<static_cast<double>(sumTab(errors,maxID))/(static_cast<double>(tab[0].size()))*100;
}

int prevRequest(vector<int> &givenVector, size_t currentPos, int &currentVal)
{
	for(int i=static_cast<int>(currentPos)-1; i>-1; --i)
	{
		if(givenVector[i]==currentVal)
			return static_cast<int>(currentPos)-i;
	}
	if(currentVal==-1)
		return static_cast<int>(givenVector.size())+1;
	return static_cast<int>(givenVector.size());
}

void lru(vector<int> tab[])
{
	fOutput<<"Algorithm: LRU\n\nstates of frames:\n";
	fOutput<<"-----------------\n";
	cout<<"states of frames:\n";
	cout<<"-----------------\n";
	const size_t maxID=vectorMax(tab[0])+1;
	// { noOfErrors[ForThisID] }
	int errors[maxID];
	for(auto &i: errors)
	{
		i=0;
	}
	// { pageID[noOfFramesFromFile] }
	int frames[static_cast<size_t>(tab[1][0])];
	for(auto &i: frames)
	{
		i=-1;
	}
	for(auto &i: tab[0])
	{
		bool isThere_=false;
		for(auto &frame: frames)
		{
			fOutput<<frame<<"\t";
			cout<<frame<<"\t";
			if(frame==i)
				isThere_=true;
		}
		fOutput<<"\n";
		cout<<"\n";
		if(!isThere_)
		{
			// { FrameIndex : futureUsageDistance }
			int toChange[2]={0,INT_MIN};
			for(int ii=0; ii<tab[1][0]; ++ii)
			{
				int toCheck=prevRequest(tab[0],&i-&tab[0][0],frames[ii]);
				if(toCheck>toChange[1])
				{
					toChange[0]=ii;
					toChange[1]=prevRequest(tab[0],&i-&tab[0][0],frames[ii]);
				}
			}
//			if(frames[toChange[0]]>-1)
			++errors[static_cast<size_t>(i)];
			frames[toChange[0]]=i;
		}
	}
	for(auto &ii: frames)
	{
		fOutput<<ii<<"\t";
		cout<<ii<<"\t";
	}

	fOutput<<"\n\nNo of page faults for every page ID:\n";
	fOutput<<"------------------------------------";
	cout<<"\n\nNo of page faults for every page ID:\n";
	cout<<"------------------------------------";
	for(auto i=0; i<maxID; ++i)
	{
		fOutput<<'\n'<<i<<":\t"<<errors[i];
		cout<<'\n'<<i<<":\t"<<errors[i];
	}
	fOutput<<"\n\n% of page faults among all requests: "<<static_cast<double>(sumTab(errors,maxID))/(static_cast<double>(tab[0].size()))*100;
	cout<<"\n\n% of page faults among all requests: "<<static_cast<double>(sumTab(errors,maxID))/(static_cast<double>(tab[0].size()))*100;
}

int appearCount(vector<int> &tab, size_t currentPos, int val)
{
	int counter=0;
	for(int i=0; i<currentPos; ++i)
	{
		if(tab[i]==val)
		{
			counter++;
		}
	}
	return counter;
}

void mfu(vector<int> tab[])
{
	fOutput<<"Algorithm: MFU\n\nstates of frames:\n";
	fOutput<<"-----------------\n";
	cout<<"states of frames:\n";
	cout<<"-----------------\n";
	const size_t maxID=vectorMax(tab[0])+1;
	// { noOfErrors[ForThisID] }
	int errors[maxID];
	for(auto &i: errors)
	{
		i=0;
	}
	// { pageID[noOfFramesFromFile] }
	int frames[static_cast<size_t>(tab[1][0])];
	for(auto &i: frames)
	{
		i=-1;
	}

	for(auto &i: tab[0])
	{
		bool isThere_=false;
		for(auto &frame: frames)
		{
			fOutput<<frame<<"\t";
			cout<<frame<<"\t";
			if(frame==i)
				isThere_=true;
		}
		if(!isThere_)
		{
			// { frameID : appearCounter }
			int toChange[2]={0,INT_MIN};
//			for(int ii=0; ii<&i-&tab[0][0];ii++)
//				cout<<"|"<<to_string(tab[0][ii])<<"|\t";

//			cout<<"\n\n\nramka\tval\til. wyst.\n";

			for(auto &frame: frames)
			{
//				cout<<to_string(&frame-&frames[0])<<"\t"<<to_string(frame)<<"\t"<<to_string(appearCount(tab[0],&i-&tab[0][0],frame))<<"\n";
				if(frame==-1)
				{
					toChange[0]=static_cast<int>(&frame-&frames[0]);
					break;
				}
				if(appearCount(tab[0],&i-&tab[0][0],frame)>toChange[1])
				{
					toChange[0]=static_cast<int>(&frame-&frames[0]);
					toChange[1]=appearCount(tab[0],&i-&tab[0][0],frame);
				}
			}
			frames[toChange[0]]=i;
			++errors[static_cast<size_t>(i)];
		}
		fOutput<<"\n";
		cout<<"\n";
	}
	for(auto &ii: frames)
	{
		fOutput<<ii<<"\t";
		cout<<ii<<"\t";
	}

	fOutput<<"\n\nNo of page faults for every page ID:\n";
	fOutput<<"------------------------------------";
	cout<<"\n\nNo of page faults for every page ID:\n";
	cout<<"------------------------------------";
	for(auto i=0; i<maxID; ++i)
	{
		fOutput<<'\n'<<i<<":\t"<<errors[i];
		cout<<'\n'<<i<<":\t"<<errors[i];
	}
	fOutput<<"\n\n% of page faults among all requests: "<<static_cast<double>(sumTab(errors,maxID))/(static_cast<double>(tab[0].size()))*100;
	cout<<"\n\n% of page faults among all requests: "<<static_cast<double>(sumTab(errors,maxID))/(static_cast<double>(tab[0].size()))*100;
}
