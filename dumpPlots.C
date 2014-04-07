void dumpPlots(){
  // Shows BR and xsec plots for different working points


  double mass[] = {
    180.,
    200.,
    220.,
    240.,
    250.,
    260.,
    280.,
    300.,
    350.,
    400.,
    500.,
    600.,
    700.
  };

  
  double tanb5BRtaunu[] = {
    0.731186,
    0.169792,
    0.080715,
    0.041369,
    0.035701,
    0.031499,
    0.026461,
    0.022798,
    0.017161,
    0.013466,
    0.006197,
    0.004783,
    0.004229
  };
  

  double tanb5BRtb[] = {
    0.241657,
    0.817053,
    0.869806,
    0.671911,
    0.664358,
    0.661665,
    0.661667,
    0.641995,
    0.583223,
    0.504500,
    0.253983,
    0.202538,
    0.180948
  };


  double tanb30BRtaunu[] = {
    0.901490,
    0.534418,
    0.367483,
    0.286135,
    0.257198,
    0.236659,
    0.210035,
    0.192875,
    0.168414,
    0.151062,
    0.110921,
    0.095882,
    0.088969
  };
  
  double tanb30BRtb[] = {
    0.081662,
    0.455760,
    0.625690,
    0.692009,
    0.705360,
    0.717428,
    0.733824,
    0.744171,
    0.750776,
    0.722967,
    0.563428,
    0.495196,
    0.459958
  };
  
  double xsecMasses[] = {
    200.,
    220.,
    240.,
    260.,
    280.,
    300.,
    320.,
    340.,
    360.,
    380.,
    400.,
    420.,
    440.,
    460.,
    480.,
    500.,
    520.,
    540.,
    560.,
    580.,
    600.
  };
  
  double tanb5xsec[] = {
    0.0562722 ,
    0.045371  ,
    0.0371716 ,
    0.0305609 ,
    0.0251598 ,
    0.0207684 ,
    0.017252  ,
    0.0143804 ,
    0.012012  ,
    0.0101007 ,
    0.00848145,
    0.00714658,
    0.00606376,
    0.00514916,
    0.00437617,
    0.00374857,
    0.00321585,
    0.00275306,
    0.00236844,
    0.00204888,
    0.00177497
  };
  
  double tanb30xsec[] = {
    0.127996  ,
    0.102709  ,
    0.0838679 ,
    0.068622  ,
    0.0564418 ,
    0.0464345 ,
    0.0384853 ,
    0.032072  ,
    0.0267729 ,
    0.0224387 ,
    0.0188167 ,
    0.015822  ,
    0.0134227 ,
    0.0113782 ,
    0.0096709 ,
    0.00827089,
    0.00708618,
    0.00605507,
    0.00520774,
    0.00449954,
    0.00389719
  };

  // Create the graphs
  
//  if(mass->size() != tanb5BRtaunu->size())  cout << "Alert: tanb5brtaunu size problem" << endl;
//  if(mass->size() != tanb5BRtb->size())     cout << "Alert: tanb5brtb size problem" << endl;
//  if(mass->size() != tanb30BRtaunu->size()) cout << "Alert: tanb30brtaunu size problem" << endl;
//  if(mass->size() != tanb30BRtb->size())    cout << "Alert: tanb30brtb size problem" << endl;
//
//  if(xsecMasses.size() != tanb5xsec.size())     cout << "Alert: tanb5xsec size problem" << endl;
//  if(xsecMasses.size() != tanb30xsec.size())    cout << "Alert: tanb30xsec size problem" << endl;

  
  TGraph* g_tanb5BRtaunu  = new TGraph(13, mass, tanb5BRtaunu );
  TGraph* g_tanb5BRtb     = new TGraph(13, mass, tanb5BRtb );
  TGraph* g_tanb30BRtaunu = new TGraph(13, mass, tanb30BRtaunu );
  TGraph* g_tanb30BRtb    = new TGraph(13, mass, tanb30BRtb );
  TGraph* g_tanb5xsec     = new TGraph(21, xsecMasses, tanb5xsec );
  TGraph* g_tanb30xsec    = new TGraph(21, xsecMasses, tanb30xsec );
                                              
  g_tanb5BRtaunu ->SetLineColor(kRed);    g_tanb5BRtaunu ->SetLineStyle(2); g_tanb5BRtaunu ->SetLineWidth(3); g_tanb5BRtaunu ->SetTitle("BR(H^{#pm}#rightarrow#tau#nu), tan#beta=5");   
  g_tanb5BRtb    ->SetLineColor(kRed-2);  g_tanb5BRtb    ->SetLineStyle(3); g_tanb5BRtb    ->SetLineWidth(3); g_tanb5BRtb    ->SetTitle("BR(H^{#pm}#rightarrow tb), tan#beta=5"); 
  g_tanb30BRtaunu->SetLineColor(kBlue);   g_tanb30BRtaunu->SetLineStyle(2); g_tanb30BRtaunu->SetLineWidth(3); g_tanb30BRtaunu->SetTitle("BR(H^{#pm}#rightarrow#tau#nu), tan#beta=30"); 
  g_tanb30BRtb   ->SetLineColor(kBlue-2); g_tanb30BRtb   ->SetLineStyle(3); g_tanb30BRtb   ->SetLineWidth(3); g_tanb30BRtb   ->SetTitle("BR(H^{#pm}#rightarrow tb), tan#beta=30"); 
  g_tanb5xsec    ->SetLineColor(kRed);    g_tanb5xsec    ->SetLineStyle(2); g_tanb5xsec    ->SetLineWidth(3); g_tanb5xsec    ->SetTitle("H^{#pm} prod. xsec, tan#beta=5"); 
  g_tanb30xsec   ->SetLineColor(kBlue);   g_tanb30xsec   ->SetLineStyle(2); g_tanb30xsec   ->SetLineWidth(3); g_tanb30xsec   ->SetTitle("H^{#pm} prod. xsec, tan#beta=30"); 
  
  g_tanb5BRtaunu ->GetXaxis()->SetTitle("H^{#pm} mass [GeV]"); g_tanb5BRtaunu ->GetYaxis()->SetTitle("Branching ratio");
  g_tanb5BRtb    ->GetXaxis()->SetTitle("H^{#pm} mass [GeV]"); g_tanb5BRtb    ->GetYaxis()->SetTitle("Branching ratio");
  g_tanb30BRtaunu->GetXaxis()->SetTitle("H^{#pm} mass [GeV]"); g_tanb30BRtaunu->GetYaxis()->SetTitle("Branching ratio");
  g_tanb30BRtb   ->GetXaxis()->SetTitle("H^{#pm} mass [GeV]"); g_tanb30BRtb   ->GetYaxis()->SetTitle("Branching ratio");
  g_tanb5xsec    ->GetXaxis()->SetTitle("H^{#pm} mass [GeV]"); g_tanb5xsec    ->GetYaxis()->SetTitle("Cross section [pb]");
  g_tanb30xsec   ->GetXaxis()->SetTitle("H^{#pm} mass [GeV]"); g_tanb30xsec   ->GetYaxis()->SetTitle("Cross section [pb]");
  
  
  
  TLegend* leg1 = new TLegend(0.7,0.8,0.9, 0.9);
  leg1->AddEntry(g_tanb5BRtaunu ,g_tanb5BRtaunu  ->GetTitle(), "l");
  leg1->AddEntry(g_tanb5BRtb    ,g_tanb5BRtb     ->GetTitle(), "l");
  leg1->AddEntry(g_tanb30BRtaunu,g_tanb30BRtaunu ->GetTitle(), "l");
  leg1->AddEntry(g_tanb30BRtb   ,g_tanb30BRtb    ->GetTitle(), "l");
  
  TLegend* leg2 = new TLegend(0.7,0.8,0.9, 0.9);
  leg2->AddEntry(g_tanb5xsec , g_tanb5xsec ->GetTitle(), "l");   
  leg2->AddEntry(g_tanb30xsec, g_tanb30xsec->GetTitle(), "l");   

  leg1->SetFillColor(0);
  leg2->SetFillColor(0);

  gStyle->SetOptTitle(0);

  TPaveText *pt = new TPaveText(0.15,0.93,0.9,1.0, "brNDC");
  pt->SetBorderSize(1);
  pt->SetFillColor(19);
  pt->SetFillStyle(0);
  pt->SetLineColor(0);
  //  pt->SetTextFont(132);                                                                                                                                                              
  pt->SetTextSize(0.045);
  TText *text = pt->AddText("mhmodp model, #sqrt{s} = 8 TeV");
  text->SetTextAlign(11);


  TCanvas* brs = new TCanvas("brs", "branching ratios", 2000, 2000 );
  brs->cd();
  gPad->SetLogy();
  g_tanb30BRtaunu->SetMaximum(2.);
  g_tanb30BRtaunu->SetMinimum(0.003);
  g_tanb30BRtaunu ->Draw("ac*");
  g_tanb5BRtb    ->Draw("c*same");
  g_tanb5BRtaunu->Draw("c*same");
  g_tanb30BRtb   ->Draw("c*same");
  leg1->Draw();
  pt->Draw();
  brs->Modified();
  brs->Update();
  brs->SaveAs("branchingRatiosReference.png");
  brs->SaveAs("branchingRatiosReference.pdf");

  TCanvas* xsecs = new TCanvas("xsecs", "cross sections", 2000, 2000);
  xsecs->cd();
  gPad->SetLogy();
  g_tanb30xsec->SetMinimum(0.001);
  g_tanb30xsec->GetYaxis()->SetTitleOffset(1.3);
  g_tanb30xsec ->Draw("ac*");
  g_tanb5xsec->Draw("c*same");
  leg2->Draw();
  pt->Draw();
  xsecs->Modified();
  xsecs->Update();
  xsecs->SaveAs("crossSectionsReference.png");
  xsecs->SaveAs("crossSectionsReference.pdf");

}
