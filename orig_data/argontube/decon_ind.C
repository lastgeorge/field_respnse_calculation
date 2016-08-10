void decon_ind(Int_t chan = 221){
  TFile *file = new TFile("Run_00009188.root");
  TH2* h1 = (TH2*)file->Get(Form("Ind_%d",chan));
  TH2* h3 = (TH2*)h1->Clone("h3");
  TH2* h4 = (TH2*)h1->Clone("h4");
  
  h3->Reset();
  h4->Reset();

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","c1",800,800);
  c1->Divide(2,2);
  c1->cd(1);
  h1->Draw("COLZ");
  h1->GetZaxis()->SetRangeUser(-80,80);
  h1->GetXaxis()->SetRangeUser(0,3000);

  h1->SetTitle("Original Induction");
  h3->SetTitle("Noise Removed Induction");
  
  Int_t max_bin = h1->GetMaximumBin();
  Float_t max = h1->GetBinContent(max_bin);
  Int_t min_bin = h1->GetMinimumBin();
  Float_t min = h1->GetBinContent(min_bin);
  
  TH1F *h2 = new TH1F("h2","h2",Int_t(max - min+2), min-1, max+1);
  
  //cout << h1->GetXaxis()->GetNbins() << " " << h1->GetYaxis()->GetNbins() << endl;
  for (Int_t i=0;i!=h1->GetXaxis()->GetNbins();i++){
    h2->Reset();
    for (Int_t j=0;j!=h1->GetYaxis()->GetNbins();j++){
      h2->Fill(h1->GetBinContent(i,j));
    }
    Double_t xq = 0.5;
    Double_t par[10];
    h2->GetQuantiles(1,&par[0],&xq);
    
    for (Int_t j=0;j!=h1->GetYaxis()->GetNbins();j++){
      h3->SetBinContent(i,j,h1->GetBinContent(i,j)-par[0]);
    }
    //cout << par[0] << endl;
    
  }
  c1->cd(2);
  h3->Draw("COLZ");
  h3->GetXaxis()->SetRangeUser(0,3000);
  

  // do deconvolution for every channel ...
  c1->cd(3);
  TFile *file1 = new TFile("ave_res.root");
  TH1F *hva = (TH1F*)file1->Get("hu");
  TGraph *gva = new TGraph();
  for (Int_t i=0;i!=hva->GetNbinsX();i++){
    Double_t x = hva->GetBinCenter(i+1);
    Double_t y = hva->GetBinContent(i+1) * (-1);
    gva->SetPoint(i,x,y);
  }
  TH1F *h2t = new TH1F("h2t","h2t",h1->GetXaxis()->GetNbins(),0,h1->GetXaxis()->GetNbins());
  TH1F *h2r = (TH1F*)h2t->Clone("h2r");
  h2r->Reset();
  for (Int_t i=0;i!=h2r->GetNbinsX();i++){
     Double_t x = h2r->GetBinCenter(i+1)/2.-50;
     h2r->SetBinContent(i+1,gva->Eval(x));
  }
  TF1 *filter_v = new TF1("filter_v","(x>0.0)*gaus*exp(-0.5*pow(x/[3],[4]))");
  double par1[5]={1.74/0.941034, 1.46, 1.33, 0.23, 4.89};
  filter_v->SetParameters(par1);

  TVirtualFFT::SetTransform(0);
  TH1 *hmr = 0;
  TH1 *hpr = 0;
  Int_t n = 8192;
  TVirtualFFT *ifft = TVirtualFFT::FFT(1,&n,"C2R M K");
  Double_t value_re[8192];
  Double_t value_im[8192];
  TH1 *fb = 0;
  TH1 *hmv = 0;
  TH1 *hpv = 0;

  for (Int_t k=0;k!=h1->GetYaxis()->GetNbins();k++){
  // for (Int_t k=33;k!=33+1;k++){
    h2t->Reset();
    for (Int_t i=0;i!=8192;i++){
      float content = h3->GetBinContent(i+1,k+1);
      h2t->SetBinContent(i+1,content);
    }
    h2t->Draw();

    cout << h2t->Integral(500,1500) << endl;
    //cout << max << " " << min << endl;
    hmr = 0;
    hpr = 0;
    hmr = h2r->FFT(hmr,"MAG");
    hpr = h2r->FFT(hpr,"PH");
    
    hmv = 0;
    hpv = 0;
    hmv = h2t->FFT(hmv,"MAG");
    hpv = h2t->FFT(hpv,"PH");
    
    for (Int_t j=0;j!=8192;j++){
      Double_t freq;
      if ( j < 8192/2.){
	freq = j/8192.*2.;
      }else{
	freq = (8192-j)/8192.*2.;
      }
      Double_t rho;
      if (hmr->GetBinContent(j+1)!=0){
	rho = hmv->GetBinContent(j+1) / hmr->GetBinContent(j+1)*filter_v->Eval(freq);
      }else{
	rho = 0;
      }
      Double_t phi = hpv->GetBinContent(j+1) - hpr->GetBinContent(j+1);
      value_re[j] = rho*cos(phi)/8192.;
      value_im[j] = rho*sin(phi)/8192.;
    }
    
    ifft->SetPointsComplex(value_re,value_im);
    ifft->Transform();
    fb = 0;
    fb = TH1::TransformHisto(ifft,fb,"Re");
    
    Double_t sum = 0;
    Double_t sum1 = 0;
    for (Int_t i=5000;i!=8000;i++){
      sum += fb->GetBinContent(i+1);
      sum1 += 1;
    }
    

    for (Int_t i=0;i!=8192;i++){
      Int_t binnum = i+1+104;
      if (binnum > 8192) binnum -=8192;
      h4->SetBinContent(binnum,k+1,fb->GetBinContent(i+1)-sum/sum1);
    }
  }


  h3->GetZaxis()->SetRangeUser(0,100);

  c1->cd(3);
  h4->RebinX(4);
  for (Int_t i=0;i!=h4->GetXaxis()->GetNbins();i++){
    for (Int_t j=0;j!=h4->GetYaxis()->GetNbins();j++){
      Double_t content1 = h4->GetBinContent(i+1,j+1);
      // if (content1 <1)
  	//	h4->SetBinContent(i+1,j+1,1);
    }
  }
  h4->Draw("COLZ");
  h4->GetZaxis()->SetRangeUser(0,30);
  h4->GetXaxis()->SetRangeUser(0,3000);
  c1->cd(4);
  TH2 *h5 = (TH2*)file->Get(Form("Col_%d",chan));
  h5->Draw("COLZ");
  h5->GetZaxis()->SetRangeUser(0,300);
  h5->GetXaxis()->SetRangeUser(0,3000);
  c1->cd(2);
  for (Int_t i=0;i!=h3->GetXaxis()->GetNbins();i++){
    for (Int_t j=0;j!=h3->GetYaxis()->GetNbins();j++){
      Double_t content1 = h3->GetBinContent(i+1,j+1);
      content1 = fabs(content1);
      h3->SetBinContent(i+1,j+1,content1);
    }
  }
  h3->Draw("COLZ");

  h4->SetTitle("Deconvoluted Induction");
  h5->SetTitle("Collection Raw");
  
  // c1->cd(1);
  // h2r->Draw();

  // c1->cd(4);
  // Double_t sum = 0;
  // Double_t sum1 = 0;
  // for (Int_t i=4000;i!=7000;i++){
  //   sum += fb->GetBinContent(i+1);
  //   sum1 += 1;
  // }
  // TH1F *h2k = (TH1F*)h2t->Clone("h2k");
  // for (Int_t j=0;j!=8192;j++){
  //   h2k->SetBinContent(j+1,fb->GetBinContent(j+1)-sum/sum1);
  // }
  // h2k->Rebin(4);
  // h2k->Draw();
  

}
