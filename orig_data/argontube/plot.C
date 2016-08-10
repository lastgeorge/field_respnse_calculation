void plot(){
  TFile *file = new TFile("Run_00009188.root");
  TH2* h1 = (TH2*)file->Get("Ind_221");

  TCanvas *c1 = new TCanvas("c1","c1",800,800);
  c1->Divide(2,2);
  c1->cd(1);
  h1->Draw("COLZ");
  h1->GetZaxis()->SetRangeUser(-30,30);
  
  c1->cd(2);
  TH1F *h2 = new TH1F("h2","h2",8192,0,8192);
  for (Int_t i=0;i!=8192;i++){
    float content = h1->GetBinContent(i,30);
    h2->SetBinContent(i+1,content);
  }
  h2->Draw();
  TFile *file1 = new TFile("ave_res_orig.root");
  TH1F *hva = (TH1F*)file1->Get("hv");
  TGraph *gva = new TGraph();
  for (Int_t i=0;i!=hva->GetNbinsX();i++){
    Double_t x = hva->GetBinCenter(i+1);
    Double_t y = hva->GetBinContent(i+1);
    gva->SetPoint(i,x,y);
  }
  
  TH1F *h2r = (TH1F*)h2->Clone("h2r");
  h2r->Reset();
  for (Int_t i=0;i!=h2r->GetNbinsX();i++){
     Double_t x = h2r->GetBinCenter(i+1)/2.-50;
     h2r->SetBinContent(i+1,gva->Eval(x));
  }
  TF1 *filter_v = new TF1("filter_v","(x>0.0)*gaus*exp(-0.5*pow(x/[3],[4]))");
  double par1[5]={1.74/0.941034, 1.46, 1.33, 0.23, 4.89};
  filter_v->SetParameters(par1);
  //h2r->Draw();
  
  TVirtualFFT::SetTransform(0);
  TH1 *hmr = 0;
  TH1 *hpr = 0;
  Int_t n = 8192;
  TVirtualFFT *ifft = TVirtualFFT::FFT(1,&n,"C2R M K");
  Double_t value_re[8192];
  Double_t value_im[8192];
  TH1 *fb = 0;

  hmr = 0;
  hpr = 0;
  hmr = h2r->FFT(hmr,"MAG");
  hpr = h2r->FFT(hpr,"PH");

  TH1 *hmv = 0;
  TH1 *hpv = 0;
  hmv = h2->FFT(hmv,"MAG");
  hpv = h2->FFT(hpv,"PH");
  
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

    c1->cd(2);
    h2->Draw();
    c1->cd(3);
    h2r->Draw();

    c1->cd(4);
    
    TH1F *h2k = (TH1F*)h2->Clone("h2k");
    
    for (Int_t j=0;j!=8192;j++){
      h2k->SetBinContent(j+1,fb->GetBinContent(j+1));
    }

    h2k->Draw();
}
