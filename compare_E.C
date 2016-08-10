void compare_E(){
  TFile *file1 = new TFile("3mm/3mm.root");
  TGraph *g1 = (TGraph*)file1->Get("gvc_1");
  TFile *file2 = new TFile("uboone.root");
  TGraph *g2 = (TGraph*)file2->Get("gvc_4");
  
  g1->Draw("AL");
  g2->Draw("Lsame");
  g2->SetLineColor(2);
  
  TLegend *le1 =new TLegend(0.6,0.6,0.89,0.89);
  le1->SetFillColor(10);
  le1->AddEntry(g1,"500 V/cm","l");
  le1->AddEntry(g2,"273 V/cm","l");
  le1->Draw();
}
