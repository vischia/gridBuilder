#include <iostream>

#include <TSystem.h>
#include <TStyle.h>



// Compile me: .L teeenyWeenyModelBuilder.C
// Run me: teenyWeenyModelBuilder("--blah")

using namespace std;

TStyle* setStyle();

void drawPlot(TH1F* h);

void draw2DPlot(TH2F* h, bool log=true);

void teenyWeenyModelBuilder(TString action){

  TStyle* myStyle=NULL;
  myStyle= setStyle();
  myStyle->cd();


  gSystem->Exec("mkdir -p outPlots");



  // X axis is always tanbeta

  Double_t xbinsTanb[29] = {
    0.,
    1.,
    2.,
    3.,
    4.,
    5.,
    6.,
    7.,
    8.,
    9.,
    10.,
    11.,
    12.,
    13.,
    14.,
    15.,
    16.,
    17.,
    18.,
    19.,
    20.,
    25.,
    30.,
    35.,
    40.,
    45.,
    50.,
    55.,
    60
  };
  ///////  h->RebinX(28,"hnew",xbinsRebin);  //creates a new variable bin size histogram hnew


  if(action=="--help")
    {

      cout << "Actions: download, deltab, brs, xsecs, susycorrs, finalfile" << endl;

      cout << "    The xsec is proportional to mt/tb + mb*tb; with delta_b corrections: mt/tb + mb*tb/(1 + delta_b).	   " << endl;
      cout << "													   " << endl;
      cout << "    mt/tb + mb*tb/(1 + delta_b)									   " << endl;
      cout << "      = 1/sqrt{1 + delta_b} (mt/tb sqrt{1 + delta_b} + mb*tb/sqrt{1 + delta_b})			   " << endl;
      cout << "	= 1/sqrt{1 + delta_b} (mt/tbeff + mt*tbeff)							   " << endl;
      cout << "													   " << endl;
      cout << "	  with tbeff = tb/sqrt{1 + delta_b}.								   " << endl;
      cout << "--------------------------------------------------------------------------------------------------------" << endl; 
      cout << "Procedure: " << endl;
      cout << "Choose a point with charged Higgs mass mhp and tan(beta) tb" << endl;
      cout << "From the deltabeta model file, find the delta_b value corresponding to tb" << endl;
      cout << "Calculate the effective tan(beta): tbeff = tb/sqrt{1 + delta_b}" << endl;
      cout << "From the MSSM grid without SUSY-QCD NLO corrections, get the cross section xs which corresponds to *tbeff* (!) " << endl;
      cout << "Calculate the cross section to be used: xs_final = xs  * 1/(1+delta_b)" << endl;
      cout << "(Note that this typically is not sufficient at low tan beta, where other SQCD-related corrections on top of delta_b corrections are not negligible. There is no official recipe on how to deal with this, but a conservative way would be to assign an extra relative uncertainty of 10% for tan beta<10 -- but of course these additional contributions depend heavily on the scenario." << endl;
      return;
    }
  else if(action=="--download")
    { 
      cout << "Getting cross sections grid... ";
      gSystem->Exec("wget https://twiki.cern.ch/twiki/pub/LHCPhysics/MSSMCharged/grid_8tev.txt --no-check-certificate");
      cout << "Done." << endl; 
      
      cout << "Getting scenarios, deltabs and branching ratios... ";
      gSystem->Exec("wget --no-check-certificate https://twiki.cern.ch/twiki/pub/LHCPhysics/MSSMCharged/scenarios_feynhiggs.tar");
      gSystem->Exec("wget --no-check-certificate https://twiki.cern.ch/twiki/pub/LHCPhysics/MSSMCharged/deltab.tar");
      //    https://twiki.cern.ch/twiki/bin/view/LHCPhysics/BRs -> MSSM YR3 numbers
      gSystem->Exec("wget --no-check-certificate https://twiki.cern.ch/twiki/pub/LHCPhysics/BRs/BR.mhmodp.Hp.output");
      cout << "Done." << endl;
      cout << "Unpacking... ";
      gSystem->Exec("tar xvf scenarios_feynhiggs.tar");
      gSystem->Exec("tar xvf deltab.tar");
      cout << "Done." << endl;

      
    }
  else if(action=="--deltab")
    {
      // Read data from deltab values file
      // (evolved from http://root.cern.ch/root/html/tutorials/tree/basic.C.html )
      TString model("mhmodp");
      ifstream in;
      in.open(Form("mssm_%s-deltab.txt",model.Data()));
      // test file:  in.open("/afs/cern.ch/cms/slc5_amd64_gcc472/lcg/root/5.34.03-cms4//tutorials/tree/basic.dat");   
      //   Float_t x,y,z;
      Float_t 
	temp,
	deltab,
	tanb;
      Int_t nlines(0);

      TFile* f = new TFile("testDeltaBeta.root","RECREATE");  
  
      TH1F* deltabVsTanb = new TH1F("deltabVsTanb","Delta b as a function of tanb",100, 0., 100.);
      deltabVsTanb->GetXaxis()->SetTitle("tan#beta");
      deltabVsTanb->GetYaxis()->SetTitle("#delta#beta");
      // TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x:y:z");
      while (1) {
	in >> temp >> tanb >> deltab;
	if (!in.good()) break;
	printf("ordinalNumber=%8f, tanb=%8f, deltab=%8f\n",temp,tanb,deltab);
	deltabVsTanb->Fill(tanb,deltab);
	//ntuple->Fill(x,y,z);
	nlines++;
      }
      printf(" found %d points\n",nlines);

      drawPlot(deltabVsTanb);
      
      in.close();
      f->Write();
      
    
    }
  else if(action=="--brs")
    {
      //https://twiki.cern.ch/twiki/pub/LHCPhysics/BRs/DR-script-final-README
      // 
      // output file BR.03(04).Hp.output:
      //  - each line corresponds to one point in the phase space
      //  - entries are M_A - tanbeta - mue - M_p - totWidth - BRenue - BRmunumu - BRtaunutau - BRtbb 
      //    - BRtsb - BRtdb - BRcbb - BRcsb - BRcdb - BRubb - BRusb - BRudb - BRH0W - BRHHW - BRA0W - BRSUSY
      //  For problems in the format of downloaded input files report problems to daniela.rebuzzi@cern.ch and to Sven.Heinemeyer@cern.ch
      
      double     ,
	M_A        ,
	tanbeta    ,
	mue 	   ,
	M_p 	   ,
	totWidth   ,
	BRenue 	   ,
	BRmunumu   ,
	BRtaunutau ,
	BRtbb 	   ,
	BRtsb 	   ,
	BRtdb 	   ,
	BRcbb 	   ,
	BRcsb 	   ,
	BRcdb 	   ,
	BRubb 	   ,
	BRusb 	   ,
	BRudb 	   ,
	BRH0W 	   ,
	BRHHW 	   ,
	BRA0W 	   ,
	BRSUSY     ;    
  
      TString model("mhmodp");
      ifstream in;
      in.open(Form("BR.%s.Hp.output",model.Data()));

      TFile* f = new TFile("testBRs.root","RECREATE");  
  
      Int_t nlines(0);
      TH2F* tbh = new TH2F("TBH","B(H->taunu) in (tanb,M_{A}) plane", 1000, 0., 1000., 28, xbinsTanb);
      tbh->GetXaxis()->SetTitle("tan#beta");
      tbh->GetYaxis()->SetTitle("M_{A} [GeV]");
      TH2F* htb = new TH2F("HTB","B(H->tb) in (tanb,M_{A}) plane", 1000, 0., 1000., 28, xbinsTanb);
      htb->GetXaxis()->SetTitle("tan#beta");
      htb->GetYaxis()->SetTitle("M_{A} [GeV]");
      TH2F* ma = new TH2F("ma", "mA(mhp,tanbeta)", 1000, 0., 1000., 28, xbinsTanb);
      ma->GetXaxis()->SetTitle("tan#beta");
      ma->GetYaxis()->SetTitle("M_{H^{#pm}} [GeV]");

      // TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x:y:z");                                                                      
      while (1) {
	in 
	>> M_A      
	>> tanbeta  
	>> mue 	   
	>> M_p 	   
	>> totWidth
	>> BRenue 	
	>> BRmunumu   
	>> BRtaunutau 
	>> BRtbb 	   
	>> BRtsb 	   
	>> BRtdb 	   
	>> BRcbb 	   
	>> BRcsb 	   
	>> BRcdb 	   
	>> BRubb 	   
	>> BRusb 	   
	>> BRudb 	   
	>> BRH0W 	   
	>> BRHHW 	   
	>> BRA0W 	   
	>> BRSUSY     ;    


        if (!in.good()) break;
        if(tanbeta==30) printf("M_A=%8f, tanbeta=%8f, mue=%8f, M_Hp =%8f, BR(H->taunu)=%8f, BR(H->tb)=%8f\n",M_A,tanbeta,mue,M_p,BRtaunutau,BRtbb);
	tbh->Fill(M_A,tanbeta,BRtaunutau);
	htb->Fill(M_A,tanbeta,BRtbb);
	ma->Fill(M_p,tanbeta,M_A);
	
        //ntuple->Fill(x,y,z);                                                                                                                        
        nlines++;
      }
      printf(" found %d points\n",nlines);
      
      cout << "TBH" << endl;
      for(int xa=0; xa<tbh->GetXaxis()->GetNbins(); ++xa){
	for(int ya=0; ya<tbh->GetYaxis()->GetNbins(); ++ya){
	  if(tbh->GetBinContent(xa,ya)>1.) cout << "BIN x=" << xa << ", y=" << ya << ", content= " <<tbh->GetBinContent(xa,ya)<< endl;
	}
      }
      
      cout << "HTB" << endl;
      for(int xa=0; xa<htb->GetXaxis()->GetNbins(); ++xa){
	for(int ya=0; ya<htb->GetYaxis()->GetNbins(); ++ya){
	  if(htb->GetBinContent(xa,ya)>1.) cout << "BIN x=" << xa << ", y=" << ya << ", content= " <<htb->GetBinContent(xa,ya)<< endl;
	}
      }
      
      draw2DPlot(tbh,false);
      draw2DPlot(htb,false);
      draw2DPlot(ma,false);
      in.close();
      f->Write();
     
    }
  else if(action=="--xsecs")
    {
      // Read data from deltab values file
      // (evolved from http://root.cern.ch/root/html/tutorials/tree/basic.C.html )
      ifstream in;
      in.open("grid_8tev.txt");
      Float_t 
	mhp,
	tanb,
	xsec,
	uncm,
	uncp;

      Int_t nlines(0);

      TFile* f = new TFile("testXsecs.root","RECREATE");  
  
      TH2F* mhpVsTanb = new TH2F("mhpVsTanb","Charged Higgs mass as a function of tanb", 28, xbinsTanb, 1000, 0., 1000.);
      mhpVsTanb->GetXaxis()->SetTitle("tan#beta");
      mhpVsTanb->GetYaxis()->SetTitle("M_{H^{+}} [GeV]");
      TH2F* mhpVsTanbD = new TH2F("mhpVsTanbD","Charged Higgs mass as a function of tanb",28, xbinsTanb, 21, 200., 620.);
//      TH2F* mhpVsTanbD = new TH2F("mhpVsTanbD","Charged Higgs mass as a function of tanb",61, 0., 61., 21, 200., 620.);
      mhpVsTanbD->GetXaxis()->SetTitle("tan#beta");
      mhpVsTanbD->GetYaxis()->SetTitle("M_{H^{+}} [GeV]");

      // TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x:y:z");
      while (1) {
	in >> mhp >> tanb >> xsec >> uncm >> uncp;
	if (!in.good()) break;
	printf("mhp=%8f, tanb=%8f, xsec=%8f\n",mhp,tanb,xsec);
	mhpVsTanb->Fill(tanb,mhp,xsec);
	mhpVsTanbD->Fill(tanb,mhp,xsec);
	//ntuple->Fill(x,y,z);
	nlines++;
      }
      printf(" found %d points\n",nlines);

      draw2DPlot(mhpVsTanbD);
      
      in.close();
      f->Write();
      
    
    }
  else if(action=="--susycorrs")
    {
      cout << "Calculating effective cross section" << endl;

      TFile* fdeltab = new TFile("testDeltaBeta.root","READ");
      TH1F* deltabVsTanb = (TH1F*) fdeltab->Get("deltabVsTanb"); //,"Delta b as a function of tanb",100, 0., 100., 1000, 0., 0.3);
      
      TFile* fxsec = new TFile("testXsecs.root","READ");  	  
      TH2F* mhpVsTanb = (TH2F*) fxsec->Get("mhpVsTanb");  



      TFile* f = new TFile("testCorrXsecs.root","RECREATE");  

      TH2F* mhpVsTanbS = new TH2F("mhpVsTanbS","Charged Higgs mass as a function of tanb",28, xbinsTanb, 1000, 0., 1000.);
      mhpVsTanbS->GetXaxis()->SetTitle("tan#beta");
      mhpVsTanbS->GetYaxis()->SetTitle("M_{H^{+}} [GeV]");
      TH2F* mhpVsTanbSD = new TH2F("mhpVsTanbDS","Charged Higgs mass as a function of tanb",28, xbinsTanb, 21, 200., 620.);
      mhpVsTanbSD->GetXaxis()->SetTitle("tan#beta");
      mhpVsTanbSD->GetYaxis()->SetTitle("M_{H^{+}} [GeV]");

      
      for(int tb=1; tb<61; ++tb)   // Test up to tan(beta)=50 
	{
	  cout << "tan(beta) = " << tb << endl;
	  
	  double delta_b(0);
	  for(int xbin=0; xbin<deltabVsTanb->GetXaxis()->GetNbins(); ++xbin){
	    if(deltabVsTanb->GetXaxis()->GetBinLowEdge(xbin) == tb){
	      delta_b = deltabVsTanb->GetBinContent(xbin);
	      cout << "Found delta_b=" << delta_b << " as corresponding to tb=" << tb << endl;
	    }
	  }
	  double tbeff( tb/sqrt(1+delta_b));
	  cout << "Computed tbeff=" << tbeff << endl;
	  
	  vector<double> mhps; mhps.clear();
	  mhps.push_back(200);
	  mhps.push_back(220);
	  mhps.push_back(240);
	  mhps.push_back(260);
	  mhps.push_back(280);
	  mhps.push_back(300);
	  mhps.push_back(320);
	  mhps.push_back(340);
	  mhps.push_back(360);
	  mhps.push_back(380);
	  mhps.push_back(400);
	  mhps.push_back(420);
	  mhps.push_back(440);
	  mhps.push_back(460);
	  mhps.push_back(480);
	  mhps.push_back(500);
	  mhps.push_back(520);
	  mhps.push_back(540);
	  mhps.push_back(560);
	  mhps.push_back(580);
	  mhps.push_back(600);

	  double uncxsec(0.);
	  double cxsec(0.);
	  int xbin( mhpVsTanb->GetXaxis()->FindBin(tbeff) );
	  for(size_t imhp=0; imhp<mhps.size(); ++imhp)
	    {
	      int ybin(mhpVsTanb->GetYaxis()->FindBin(mhps[imhp]));
	      
	      uncxsec = mhpVsTanb->GetBinContent(xbin,ybin);
	      cxsec = uncxsec / (1 + delta_b);
	      cout << "tanbeta = " << tb << ", tbeff=" << tbeff << ", mhp=" << mhps[imhp] << ", uncxsec=" << uncxsec << ", final xsec=" << cxsec << endl;
	      mhpVsTanbS->Fill(tb,mhps[imhp],cxsec);
	      mhpVsTanbDS->Fill(tb,mhps[imhp],cxsec);
	      
	    }
	  
	}
      draw2DPlot(mhpVsTanbS,false);
      draw2DPlot(mhpVsTanbSD,false);

      f->Write();

    }
  else if(action=="--finalfile")
    {
      cout << "Building file with final histograms"<< endl;

      TFile* fcorrxsec = new TFile("testCorrXsecs.root","READ");
      TFile* fbrs = new TFile("testBRs.root","READ");  

      
      TFile* f = new TFile("testFinalfile.root","RECREATE");  	  

      TH2F* tbh = (TH2F*) fbrs->Get("TBH");
      TH2F* htb = (TH2F*) fbrs->Get("HTB");
      TH2F* ma = (TH2F*) fbrs->Get("ma");
     
      TH2F* mhpVsTanb = (TH2F*) fcorrxsec->Get("mhpVsTanbDS");

      tbh->SetTitle("BR(Hp->taunu)");
      htb->SetTitle("BR(Hp->tb)");
      ma->SetTitle("M_A(mHp, tanbeta)");
      mhpVsTanb->SetTitle("xsec ->Hp (tan(beta), mHp), nlo+SUSqcd");

      tbh->SetName("h_brtaunu_Hp");
      htb->SetName("h_brtb_Hp");
      ma->SetName("h_mA");
      mhpVsTanb->SetName("h_sam_Hp");

      tbh->Write();
      htb->Write();
      ma->Write();
      mhpVsTanb->Write();
      
      f->Write();

    }
  
}

TStyle* setStyle(){
// TDR style crap - include from Utilities.cc                                                                                                                                          
TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
// For the canvas:                                                                                                                                                                     
tdrStyle->SetCanvasBorderMode(0);
tdrStyle->SetCanvasColor(kWhite);
tdrStyle->SetCanvasDefH(600); //Height of canvas                                                                                                                                       
tdrStyle->SetCanvasDefW(600); //Width of canvas                                                                                                                                        
tdrStyle->SetCanvasDefX(0);   //POsition on screen                                                                                                                                     
tdrStyle->SetCanvasDefY(0);

// For the Pad:                                                                                                                                                                        
tdrStyle->SetPadBorderMode(0);
// tdrStyle->SetPadBorderSize(Width_t size = 1);                                                                                                                                       
tdrStyle->SetPadColor(kWhite);
tdrStyle->SetPadGridX(false);
tdrStyle->SetPadGridY(false);
tdrStyle->SetGridColor(0);
tdrStyle->SetGridStyle(3);
tdrStyle->SetGridWidth(1);

// For the frame:                                                                                                                                                                      
tdrStyle->SetFrameBorderMode(0);
tdrStyle->SetFrameBorderSize(1);
tdrStyle->SetFrameFillColor(0);
tdrStyle->SetFrameFillStyle(0);
tdrStyle->SetFrameLineColor(1);
tdrStyle->SetFrameLineStyle(1);
tdrStyle->SetFrameLineWidth(1);
// For the histo:                                                                                                                                                                      
// tdrStyle->SetHistFillColor(1);                                                                                                                                                      
// tdrStyle->SetHistFillStyle(0);                                                                                                                                                      
tdrStyle->SetHistLineColor(1);
tdrStyle->SetHistLineStyle(0);
tdrStyle->SetHistLineWidth(1);
// tdrStyle->SetLegoInnerR(Float_t rad = 0.5);                                                                                                                                         
// tdrStyle->SetNumberContours(Int_t number = 20);                                                                                                                                     

tdrStyle->SetEndErrorSize(2);
//tdrStyle->SetErrorMarker(20);                                                                                                                                                        
tdrStyle->SetErrorX(0.);

tdrStyle->SetMarkerStyle(20);

//For the fit/function:                                                                                                                                                                
tdrStyle->SetOptFit(1);
tdrStyle->SetFitFormat("5.4g");
tdrStyle->SetFuncColor(2);
tdrStyle->SetFuncStyle(1);
tdrStyle->SetFuncWidth(1);

//For the date:                                                                                                                                                                        
tdrStyle->SetOptDate(0);
// tdrStyle->SetDateX(Float_t x = 0.01);                                                                                                                                               
// tdrStyle->SetDateY(Float_t y = 0.01);                                                                                                                                               

// For the statistics box:                                                                                                                                                             
tdrStyle->SetOptFile(0);
tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");                                                                                                           
tdrStyle->SetStatColor(kWhite);
tdrStyle->SetStatFont(42);
tdrStyle->SetStatFontSize(0.025);
tdrStyle->SetStatTextColor(1);
tdrStyle->SetStatFormat("6.4g");
tdrStyle->SetStatBorderSize(1);
tdrStyle->SetStatH(0.1);
tdrStyle->SetStatW(0.15);
// tdrStyle->SetStatStyle(Style_t style = 1001);                                                                                                                                       
// tdrStyle->SetStatX(Float_t x = 0);                                                                                                                                                  
// tdrStyle->SetStatY(Float_t y = 0);                                                                                                                                                  

// Margins:                                                                                                                                                                            
tdrStyle->SetPadTopMargin(0.05);
tdrStyle->SetPadBottomMargin(0.13);
tdrStyle->SetPadLeftMargin(0.16);
tdrStyle->SetPadRightMargin(0.04);

// For the Global title:                                                                                                                                                               

tdrStyle->SetOptTitle(0);
tdrStyle->SetTitleFont(42);
tdrStyle->SetTitleColor(1);
tdrStyle->SetTitleTextColor(1);
tdrStyle->SetTitleFillColor(10);
tdrStyle->SetTitleFontSize(0.05);
// tdrStyle->SetTitleH(0); // Set the height of the title box                                                                                                                          
// tdrStyle->SetTitleW(0); // Set the width of the title box                                                                                                                           
// tdrStyle->SetTitleX(0); // Set the position of the title box                                                                                                                        
// tdrStyle->SetTitleY(0.985); // Set the position of the title box                                                                                                                    
// tdrStyle->SetTitleStyle(Style_t style = 1001);                                                                                                                                      
// tdrStyle->SetTitleBorderSize(2);                                                                                                                                                    

// For the axis titles:                                                                                                                                                                

tdrStyle->SetTitleColor(1, "XYZ");
tdrStyle->SetTitleFont(42, "XYZ");
tdrStyle->SetTitleSize(0.06, "XYZ");
// tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?                                                                                                       
// tdrStyle->SetTitleYSize(Float_t size = 0.02);                                                                                                                                       
tdrStyle->SetTitleXOffset(0.9);
tdrStyle->SetTitleYOffset(1.25);
// tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset                                                                                                                

// For the axis labels:                                                                                                                                                                

tdrStyle->SetLabelColor(1, "XYZ");
tdrStyle->SetLabelFont(42, "XYZ");
tdrStyle->SetLabelOffset(0.007, "XYZ");
tdrStyle->SetLabelSize(0.05, "XYZ");
// For the axis:                                                                                                                                                                       

tdrStyle->SetAxisColor(1, "XYZ");
tdrStyle->SetStripDecimals(kTRUE);
tdrStyle->SetTickLength(0.03, "XYZ");
tdrStyle->SetNdivisions(510, "XYZ");
tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame                                                                                                      
tdrStyle->SetPadTickY(1);

// Change for log plots:                                                                                                                                                               
tdrStyle->SetOptLogx(0);
tdrStyle->SetOptLogy(0);
tdrStyle->SetOptLogz(0);

// Postscript options:                                                                                                                                                                 
tdrStyle->SetPaperSize(20.,20.);
// tdrStyle->SetLineScalePS(Float_t scale = 3);                                                                                                                                        
// tdrStyle->SetLineStyleString(Int_t i, const char* text);                                                                                                                            
// tdrStyle->SetHeaderPS(const char* header);                                                                                                                                          
// tdrStyle->SetTitlePS(const char* pstitle);                                                                                                                                          

// tdrStyle->SetBarOffset(Float_t baroff = 0.5);                                                                                                                                       
// tdrStyle->SetBarWidth(Float_t barwidth = 0.5);                                                                                                                                      
// tdrStyle->SetPaintTextFormat(const char* format = "g");                                                                                                                             
// tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);                                                                                                                         
// tdrStyle->SetTimeOffset(Double_t toffset);                                                                                                                                          
// tdrStyle->SetHistMinimumZero(kTRUE);                                                                                                                                                

tdrStyle->cd();

 return tdrStyle;

}

void drawPlot(TH1F* h){
  TCanvas* c = new TCanvas(h->GetName(), h->GetName(), 2000, 2000);
  c->cd();
  h->Draw();
  c->Modified();
  c->Update();
  c->Print("outPlots/" + TString(h->GetName()) + ".png");
  c->Print("outPlots/" + TString(h->GetName()) + ".pdf");
  c->Print("outPlots/" + TString(h->GetName()) + ".C");


}

void draw2DPlot(TH2F* h, bool log){
  TCanvas* c = new TCanvas(h->GetName(), h->GetName(), 2000, 2000);
  c->cd();
  h->GetXaxis()->SetTitleOffset(2.);
  h->GetYaxis()->SetTitleOffset(2.);

  
  
//  //create a new TH2 with your bin arrays spec
//
//
//  TH2F* rebinned = new TH2F(h->GetName()+TString("rebin"),h->GetTitle(),
//			    28,
//			    xbinsRebin,
//			    h->GetYaxis()->GetNbins(), 
//			    h->GetYaxis()->GetBinLowEdge(1), 
//			    h->GetYaxis()->GetBinLowEdge(h->GetYaxis()->GetNbins() +1 ) );
//
//  TAxis *xaxis = rebinned->GetXaxis();
//  TAxis *yaxis = rebinned->GetYaxis();
//  
//  for (int j=1;j<=yaxis->GetNbins();j++) {
//    for (int i=1;i<=xaxis->GetNbins();i++) {
//      rebinned->Fill(xaxis->GetBinCenter(i),yaxis->GetBinCenter(j),h->GetBinContent(i,j));
//    }
//  }

  //  h=rebinned;

  if(log)
    {
//      c->SetLogx();
//      c->SetLogy();
    }
  h->Draw("CONT");//LZ");//SURF3//LEGO30");
  //  h->GetXaxis()->SetNdivisions(2000);
  //  h->GetYaxis()->SetNdivisions(2000);
  h->GetXaxis()->SetMoreLogLabels();
  h->GetYaxis()->SetMoreLogLabels();

  c->Modified();
  c->Update();
  c->Print("outPlots/" + TString(h->GetName()) + ".png");
  c->Print("outPlots/" + TString(h->GetName()) + ".pdf");
  c->Print("outPlots/" + TString(h->GetName()) + ".C");

}
