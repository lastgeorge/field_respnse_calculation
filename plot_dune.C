void plot_dune(){
  TFile *file = new TFile("dune.root");
  TGraph *gu = (TGraph*)file->Get("guc_4");
  TGraph *gv = (TGraph*)file->Get("gvc_4");
  TGraph *gw = (TGraph*)file->Get("gwi_4");

  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  gw->Draw("AL");
  gu->Draw("Lsame");
  gv->Draw("Lsame");
  gw->GetXaxis()->SetTitle("Time (#mus)");
  gw->GetYaxis()->SetTitle("Current (A.U.)");
  gw->SetTitle("Field Response in DUNE for the closest wires");
  gw->GetYaxis()->SetRangeUser(-1.5,0.5);
  gu->SetLineColor(1);
  gv->SetLineColor(2);
  gw->SetLineColor(4);
  gu->SetLineWidth(2);
  gv->SetLineWidth(2);
  gw->SetLineWidth(2);

  TLegend *le1 = new TLegend(0.6,0.6,0.89,0.89);
  le1->SetFillColor(10);
  le1->AddEntry(gu,"U Ind. Plane","l");
  le1->AddEntry(gv,"V Ind. Plane","l");
  le1->AddEntry(gw,"W Col. Plane","l");
  le1->Draw();

}
