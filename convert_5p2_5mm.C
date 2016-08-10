const Int_t nwire = 5;
void convert_5p2_5mm(){
  TString prefix = "Sim_5.2";
  Int_t wire_num[nwire] = {1,2,3,4,5};
  
  TGraph **gu_i = new TGraph*[nwire];
  TGraph **gv_i = new TGraph*[nwire];
  TGraph **gw_i = new TGraph*[nwire];
  
  TGraph **gu_c = new TGraph*[nwire];
  TGraph **gv_c = new TGraph*[nwire];
  TGraph **gw_c = new TGraph*[nwire];
  
  TString filename;

   for (Int_t i=0;i!=nwire;i++){
    filename.Form("processed_data/5.2_degree_5mm/Sim_5.2_5mm_U%d.dat",wire_num[i]);
    ifstream infile(filename);
    Double_t x,y,num;
    gu_i[i] = new TGraph();
    gu_c[i] = new TGraph();
    Int_t ni = 0, nc = 0;
    while(!infile.eof()){
      infile >> num >> x >> y;
      y*=1e6;
      
      if (num == 1){
	ni++;
	gu_i[i]->SetPoint(ni,x,y);
      }else if (num==2){
	nc ++;
	gu_c[i]->SetPoint(nc,x,y);
      }
    }
    infile.close();
  }


   for (Int_t i=0;i!=nwire;i++){
     filename.Form("processed_data/5.2_degree_5mm/Sim_5.2_5mm_V%d.dat",wire_num[i]);
    ifstream infile(filename);
    Double_t x,y,num;
    gv_i[i] = new TGraph();
    gv_c[i] = new TGraph();
    Int_t ni = 0, nc = 0;
    while(!infile.eof()){
      infile >> num >> x >> y;
      y*=1e6;
      
      if (num == 1){
	ni++;
	gv_i[i]->SetPoint(ni,x,y);
      }else if (num==2){
	nc ++;
	gv_c[i]->SetPoint(nc,x,y);
      }
    }
    infile.close();
  }

  for (Int_t i=0;i!=nwire;i++){
    filename.Form("processed_data/5.2_degree_5mm/Sim_5.2_5mm_Y%d.dat",wire_num[i]);
    ifstream infile(filename);
    Double_t x,y,num;
    gw_i[i] = new TGraph();
    gw_c[i] = new TGraph();
    Int_t ni = 0, nc = 0;
    while(!infile.eof()){
      infile >> num >> x >> y;
      y *= 1e6;
      
      
      // if (ni%10==0&& num==1)
      // 	cout << ni << " " << x << " " << y << endl;

      if (num == 1){
	ni ++;
	gw_i[i]->SetPoint(ni,x,y);
      }else if (num==2){
	nc ++;
	gw_c[i]->SetPoint(nc,x,y);
      }
    }
    infile.close();
  }
  

  
  TFile *file = new TFile("Sim_5p2_5mm.root","RECREATE");
  for(Int_t i=0;i!=nwire;i++){
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
