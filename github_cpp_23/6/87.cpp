

#include <application_core/constants.h>
#include <plugin_SeedExtender/DepthFirstSearchData.h>


void DepthFirstSearchData::depthFirstSearch(Kmer root,Kmer a,int maxDepth,
	bool*edgesRequested,bool*vertexCoverageRequested,bool*vertexCoverageReceived,
	RingAllocator*outboxAllocator,int size,int theRank,StaticVector*outbox,
 int*receivedVertexCoverage,vector<Kmer>*receivedOutgoingEdges,
		int minimumCoverage,bool*edgesReceived,int wordSize,Parameters*parameters){
	if(!m_doChoice_tips_dfs_initiated){
		m_depthFirstSearchVisitedVertices.clear();
		m_depthFirstSearchVisitedVertices_vector.clear();

		
		m_depthFirstSearchVisitedVertices_vector.push_back(root);
		m_depthFirstSearchVisitedVertices_vector.push_back(a);

		m_depthFirstSearchVisitedVertices_depths.clear();
		while(m_depthFirstSearchVerticesToVisit.size()>0){
			m_depthFirstSearchVerticesToVisit.pop();
		}
		while(m_depthFirstSearchDepths.size()>0){
			m_depthFirstSearchDepths.pop();
		}
		m_maxDepthReached=false;
		m_depthFirstSearchVerticesToVisit.push(a);
		m_depthFirstSearchVisitedVertices.insert(a);
		m_depthFirstSearchDepths.push(0);
		m_depthFirstSearch_maxDepth=0;
		m_doChoice_tips_dfs_initiated=true;
		m_doChoice_tips_dfs_done=false;
		m_coverages.clear();
		(*edgesRequested)=false;
		(*vertexCoverageRequested)=false;
		#ifdef SHOW_MINI_GRAPH
		cout<<"<MiniGraph>"<<endl;
		cout<<root->idToWord(wordSize,parameters->getColorSpaceMode())<<" -> "<<a->idToWord(wordSize,parameters->getColorSpaceMode())<<endl;
		#endif
	}
	if(m_depthFirstSearchVerticesToVisit.size()>0){
		Kmer vertexToVisit=m_depthFirstSearchVerticesToVisit.top();
		if(!(*vertexCoverageRequested)){
			(*vertexCoverageRequested)=true;
			(*vertexCoverageReceived)=false;
			
			MessageUnit*message=(MessageUnit*)(*outboxAllocator).allocate(KMER_U64_ARRAY_SIZE*sizeof(MessageUnit));
			int j=0;
			vertexToVisit.pack(message,&j);
			int dest=parameters->_vertexRank(&vertexToVisit);
			
			Message aMessage(message,j,dest,RAY_MPI_TAG_REQUEST_VERTEX_COVERAGE,theRank);
			(*outbox).push_back(aMessage);
		}else if((*vertexCoverageReceived)){
			if(!(*edgesRequested)){
				m_coverages[vertexToVisit]=(*receivedVertexCoverage);
				m_depthFirstSearchVisitedVertices.insert(vertexToVisit);
				int theDepth=m_depthFirstSearchDepths.top();

				if(theDepth> m_depthFirstSearch_maxDepth){
					m_depthFirstSearch_maxDepth=theDepth;
				}
			
				
				MessageUnit*message=(MessageUnit*)(*outboxAllocator).allocate(1*sizeof(MessageUnit));
				int bufferPosition=0;
				vertexToVisit.pack(message,&bufferPosition);
				int destination=parameters->_vertexRank(&vertexToVisit);
				Message aMessage(message,bufferPosition,destination,RAY_MPI_TAG_REQUEST_VERTEX_OUTGOING_EDGES,theRank);
				(*outbox).push_back(aMessage);
				(*edgesRequested)=true;
				(*edgesReceived)=false;
			}else if((*edgesReceived)){
				Kmer vertexToVisit=m_depthFirstSearchVerticesToVisit.top();
				int theDepth=m_depthFirstSearchDepths.top();
				#ifdef ASSERT
				assert(theDepth>=0);
				assert(theDepth<=maxDepth);
				#endif
				int newDepth=theDepth+1;

				m_depthFirstSearchVerticesToVisit.pop();
				m_depthFirstSearchDepths.pop();

				for(int i=0;i<(int)(*receivedOutgoingEdges).size();i++){
					Kmer nextVertex=(*receivedOutgoingEdges)[i];
					if(m_depthFirstSearchVisitedVertices.count(nextVertex)>0){
						continue;
					}
					if(newDepth>maxDepth){
						m_maxDepthReached=true;
						continue;
					}

					if(m_depthFirstSearchVisitedVertices.size()<MAX_VERTICES_TO_VISIT){
						
						m_depthFirstSearchVisitedVertices_vector.push_back(vertexToVisit);
						m_depthFirstSearchVisitedVertices_vector.push_back(nextVertex);

						
						m_depthFirstSearchVisitedVertices_depths.push_back(newDepth);

						
						m_depthFirstSearchVerticesToVisit.push(nextVertex);
						m_depthFirstSearchDepths.push(newDepth);
					}


					#ifdef SHOW_MINI_GRAPH
					cout<<vertexToVisit->idToWord(wordSize,parameters->getColorSpaceMode())<<" -> "<<nextVertex->idToWord(wordSize,parameters->getColorSpaceMode())<<endl;
					#endif
				}
				(*edgesRequested)=false;
				(*vertexCoverageRequested)=false;
			}
		}
	}else{
		m_doChoice_tips_dfs_done=true;
		#ifdef SHOW_MINI_GRAPH
		cout<<"</MiniGraph>"<<endl;
		#endif
	}
}

void DepthFirstSearchData::depthFirstSearchBidirectional(Kmer a,int maxDepth,
	bool*edgesRequested,bool*vertexCoverageRequested,bool*vertexCoverageReceived,
	RingAllocator*outboxAllocator,int size,int theRank,StaticVector*outbox,
 int*receivedVertexCoverage,SeedingData*seedingData,
		int minimumCoverage,bool*edgesReceived,Parameters*parameters){

	#ifdef ASSERT
	int wordSize=parameters->getWordSize();
	#endif

	if(!m_doChoice_tips_dfs_initiated){
		m_outgoingEdges.clear();
		m_ingoingEdges.clear();

		m_depthFirstSearchVisitedVertices.clear();
		m_depthFirstSearchVisitedVertices_vector.clear();
		m_depthFirstSearchVisitedVertices_depths.clear();
		while(m_depthFirstSearchVerticesToVisit.size()>0){
			m_depthFirstSearchVerticesToVisit.pop();
		}
		while(m_depthFirstSearchDepths.size()>0){
			m_depthFirstSearchDepths.pop();
		}
		m_maxDepthReached=false;
		m_depthFirstSearchVerticesToVisit.push(a);
		m_depthFirstSearchDepths.push(0);
		m_depthFirstSearch_maxDepth=0;
		m_doChoice_tips_dfs_initiated=true;
		m_doChoice_tips_dfs_done=false;
		m_coverages.clear();
		(*edgesRequested)=false;
		(*vertexCoverageRequested)=false;
	}
	if(m_depthFirstSearchVerticesToVisit.size()>0){
		Kmer vertexToVisit=m_depthFirstSearchVerticesToVisit.top();

		if(!(*vertexCoverageRequested)){

			if(m_depthFirstSearchVisitedVertices.count(vertexToVisit)>0){
				m_depthFirstSearchVerticesToVisit.pop();
				m_depthFirstSearchDepths.pop();
				return;
			}

			(*vertexCoverageRequested)=true;
			(*vertexCoverageReceived)=false;
			
			MessageUnit*message=(MessageUnit*)(*outboxAllocator).allocate(KMER_U64_ARRAY_SIZE*sizeof(MessageUnit));
			int bufferPosition=0;
			vertexToVisit.pack(message,&bufferPosition);
			int dest=parameters->_vertexRank(&vertexToVisit);
			Message aMessage(message,bufferPosition,dest,RAY_MPI_TAG_REQUEST_VERTEX_COVERAGE,theRank);
			(*outbox).push_back(aMessage);
		}else if((*vertexCoverageReceived)){
			if(!(*edgesRequested)){
				m_coverages[vertexToVisit]=(*receivedVertexCoverage);

				#ifdef ASSERT
				if(m_depthFirstSearchVisitedVertices.count(vertexToVisit)>0){
					cout<<"Already visited: "<<vertexToVisit.idToWord(wordSize,parameters->getColorSpaceMode())<<" root is "<<a.idToWord(wordSize,parameters->getColorSpaceMode())<<endl;
				}
				assert(m_depthFirstSearchVisitedVertices.count(vertexToVisit)==0);
				assert(*receivedVertexCoverage>0);
				#endif

				if((*receivedVertexCoverage)>0){
					m_depthFirstSearchVisitedVertices.insert(vertexToVisit);
				}else{
					#ifdef ASSERT
					assert(false);
					#endif
					
					m_depthFirstSearchVerticesToVisit.pop();
					m_depthFirstSearchDepths.pop();
					(*edgesRequested)=false;
					(*vertexCoverageRequested)=false;
					return;
				}
				int theDepth=m_depthFirstSearchDepths.top();

				if(theDepth> m_depthFirstSearch_maxDepth){
					m_depthFirstSearch_maxDepth=theDepth;
				}
			
				
				MessageUnit*message=(MessageUnit*)(*outboxAllocator).allocate(1*sizeof(MessageUnit));
				int bufferPosition=0;
				vertexToVisit.pack(message,&bufferPosition);
				int destination=parameters->_vertexRank(&vertexToVisit);
				Message aMessage(message,bufferPosition,destination,RAY_MPI_TAG_REQUEST_VERTEX_EDGES,theRank);

				(*outbox).push_back(aMessage);
				(*edgesRequested)=true;
				(*edgesReceived)=false;
			}else if((*edgesReceived)){
				Kmer vertexToVisit=m_depthFirstSearchVerticesToVisit.top();
				int theDepth=m_depthFirstSearchDepths.top();

				#ifdef ASSERT
	
				assert(theDepth>=0);
				assert(theDepth<=maxDepth);
				#endif

				int newDepth=theDepth+1;

				m_depthFirstSearchVerticesToVisit.pop();
				m_depthFirstSearchDepths.pop();

				
				
				
				
				

				vector<Kmer > outgoingEdges=seedingData->m_SEEDING_receivedOutgoingEdges;

				for(int i=0;i<(int)outgoingEdges.size();i++){
					Kmer nextVertex=outgoingEdges[i];

					if(m_depthFirstSearchVisitedVertices.size()>=MAX_VERTICES_TO_VISIT){
						continue;
					}
					if(m_depthFirstSearchVisitedVertices.count(nextVertex)>0){
						continue;
					}
					if(newDepth>maxDepth){
						m_maxDepthReached=true;
						continue;
					}
					m_depthFirstSearchVerticesToVisit.push(nextVertex);
					m_depthFirstSearchDepths.push(newDepth);
		
					m_depthFirstSearchVisitedVertices_vector.push_back(vertexToVisit);
					m_depthFirstSearchVisitedVertices_vector.push_back(nextVertex);
					m_depthFirstSearchVisitedVertices_depths.push_back(newDepth);
				}

				#ifdef ASSERT
				if(m_outgoingEdges.count(vertexToVisit)>0){
					cout<<vertexToVisit.idToWord(wordSize,parameters->getColorSpaceMode())<<" is already in the data structure "<<m_outgoingEdges[vertexToVisit].size()<<" v. "<<outgoingEdges.size()<<endl;
				}
				assert(m_outgoingEdges.count(vertexToVisit)==0);
				#endif

				m_outgoingEdges[vertexToVisit]=outgoingEdges;

				vector<Kmer> ingoingEdges=seedingData->m_SEEDING_receivedIngoingEdges;

				for(int i=0;i<(int)ingoingEdges.size();i++){
					Kmer nextVertex=ingoingEdges[i];

					if(m_depthFirstSearchVisitedVertices.size()>=MAX_VERTICES_TO_VISIT){
						continue;
					}
					if(m_depthFirstSearchVisitedVertices.count(nextVertex)>0){
						continue;
					}
					if(newDepth>maxDepth){
						m_maxDepthReached=true;
						continue;
					}
					m_depthFirstSearchVerticesToVisit.push(nextVertex);
					m_depthFirstSearchDepths.push(newDepth);

					
					m_depthFirstSearchVisitedVertices_vector.push_back(nextVertex);
					m_depthFirstSearchVisitedVertices_vector.push_back(vertexToVisit);
					m_depthFirstSearchVisitedVertices_depths.push_back(newDepth);
				}

				(*edgesRequested)=false;
				(*vertexCoverageRequested)=false;

				#ifdef ASSERT
				assert(m_ingoingEdges.count(vertexToVisit)==0);
	
				#endif

				m_ingoingEdges[vertexToVisit]=ingoingEdges;
			}
		}
	}else{
		m_doChoice_tips_dfs_done=true;
		#ifdef SHOW_MINI_GRAPH
		cout<<"</MiniGraph>"<<endl;
		#endif
	}
}

map<Kmer,vector<Kmer> >*DepthFirstSearchData::getIngoingEdges(){
	return &m_ingoingEdges;
}

map<Kmer,vector<Kmer> >*DepthFirstSearchData::getOutgoingEdges(){
	return &m_outgoingEdges;
}

void DepthFirstSearchData::setTags(	MessageTag RAY_MPI_TAG_REQUEST_VERTEX_COVERAGE,
	MessageTag RAY_MPI_TAG_REQUEST_VERTEX_EDGES,
	MessageTag RAY_MPI_TAG_REQUEST_VERTEX_OUTGOING_EDGES
){
	this->RAY_MPI_TAG_REQUEST_VERTEX_OUTGOING_EDGES=RAY_MPI_TAG_REQUEST_VERTEX_OUTGOING_EDGES;
	this->RAY_MPI_TAG_REQUEST_VERTEX_EDGES=RAY_MPI_TAG_REQUEST_VERTEX_EDGES;
	this->RAY_MPI_TAG_REQUEST_VERTEX_COVERAGE=RAY_MPI_TAG_REQUEST_VERTEX_COVERAGE;
}

