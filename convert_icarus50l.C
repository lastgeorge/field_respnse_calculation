const Int_t nwire = 7;
void convert_icarus50l(){

  //  Double_t dis[6]={0,0.3,0.6,0.9,1.2,1.5};
  TString dis[5]={"0","0.3175","0.635","0.9525","1.27"};
  Int_t dis1[nwire] = {1,2,3,4,5,6,7};
  TString filename;
  
  TGraph **gu_i = new TGraph*[42];
  TGraph **gv_i = new TGraph*[42];
  TGraph **gw_i = new TGraph*[42];
  
  TGraph **gu_c = new TGraph*[42];
  TGraph **gv_c = new TGraph*[42];
  TGraph **gw_c = new TGraph*[42];

  for (Int_t i=0;i!=5;i++){
    for (Int_t j=0;j!=nwire;j++){
      
      filename = "./processed_data/icarus50l/3mm_" + dis[i] + Form("_U%d.dat",dis1[j]);

      //      if (dis[i] == 0){
      //	filename.Form("./processed_data/3mm/3mm_%1.0f_U%d.dat",dis[i],dis1[j]);
      //}else{
      //	filename.Form("./processed_data/3mm/3mm_%2.1f_U%d.dat",dis[i],dis1[j]);
      // }
      // std::cout << i << " " << j << " " << filename << std::endl;

      ifstream infile(filename);
      Double_t x,y,num;
      gu_i[i*nwire+j] = new TGraph();
      gu_c[i*nwire+j] = new TGraph();
      Int_t ni = 0, nc = 0;
      while(!infile.eof()){
	infile >> num >> x >> y;
	y*=1e6;
	
	if (num == 1){
	  ni++;
	  gu_i[i*nwire+j]->SetPoint(ni,x,y);
	}else if (num==2){
	  nc ++;
	  gu_c[i*nwire+j]->SetPoint(nc,x,y);
	}
      }
      infile.close();
    }
  }

  for (Int_t i=0;i!=5;i++){
    for (Int_t j=0;j!=nwire;j++){

      filename = "./processed_data/icarus50l/3mm_" + dis[i] + Form("_V%d.dat",dis1[j]);

      // if (dis[i] == 0){
      // 	filename.Form("./processed_data/3mm/3mm_%1.0f_V%d.dat",dis[i],dis1[j]);
      // }else{
      // 	filename.Form("./processed_data/3mm/3mm_%2.1f_V%d.dat",dis[i],dis1[j]);
      // }
      //    filename.Form("txt/v_%2.1f.dat",dis[i]);
      ifstream infile(filename);
      Double_t x,y,num;
      gv_i[i*nwire + j] = new TGraph();
      gv_c[i*nwire + j] = new TGraph();
      Int_t ni = 0, nc = 0;
      while(!infile.eof()){
	infile >> num >> x >> y;
	y*=1e6;
	
	if (num == 1){
	  ni++;
	  gv_i[i*nwire + j]->SetPoint(ni,x,y);
	}else if (num==2){
	  nc ++;
	  gv_c[i*nwire + j]->SetPoint(nc,x,y);
	}
      }
      infile.close();
    }
  }

  for (Int_t i=0;i!=5;i++){
    for (Int_t j=0;j!=nwire;j++){

      filename = "./processed_data/icarus50l/3mm_" + dis[i] + Form("_Y%d.dat",dis1[j]);
      // if (dis[i] == 0){
      // 	filename.Form("./processed_data/3mm/3mm_%1.0f_Y%d.dat",dis[i],dis1[j]);
      // }else{
      // 	filename.Form("./processed_data/3mm/3mm_%2.1f_Y%d.dat",dis[i],dis1[j]);
      // }
    //    filename.Form("txt/y_%2.1f.dat",dis[i]);
      ifstream infile(filename);
      Double_t x,y,num;
      gw_i[i*nwire+j] = new TGraph();
      gw_c[i*nwire+j] = new TGraph();
      Int_t ni = 0, nc = 0;
      while(!infile.eof()){
	infile >> num >> x >> y;
	y *= 1e6;
		
	// if (ni%10==0&& num==1)
	// 	cout << ni << " " << x << " " << y << endl;
	
	if (num == 1){
	  ni ++;
	  gw_i[i*nwire+j]->SetPoint(ni,x,y);
	}else if (num==2){
	  nc ++;
	  gw_c[i*nwire+j]->SetPoint(nc,x,y);
	}
      }
      infile.close();
    }
  }
  

  
  TFile *file = new TFile("icarus50l.root","RECREATE");
  for(Int_t i=0;i!=35;i++){
    gu_i[i]->Write(Form("gui_%d",i+1));
    gu_c[i]->Write(Form("guc_%d",i+1));

    gv_i[i]->Write(Form("gvi_%d",i+1));
    gv_c[i]->Write(Form("gvc_%d",i+1));

    gw_i[i]->Write(Form("gwi_%d",i+1));
    gw_c[i]->Write(Form("gwc_%d",i+1));
  }
  file->Write();
  file->Close();
  
  
}
