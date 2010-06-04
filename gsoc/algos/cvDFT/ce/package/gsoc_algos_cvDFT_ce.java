/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-u14
 */
import java.util.*;
import org.mozilla.javascript.*;
import xdc.services.intern.xsr.*;
import xdc.services.spec.*;

public class gsoc_algos_cvDFT_ce
{
    static final String VERS = "@(#) xdc-u14\n";

    static final Proto.Elm $$T_Bool = Proto.Elm.newBool();
    static final Proto.Elm $$T_Num = Proto.Elm.newNum();
    static final Proto.Elm $$T_Str = Proto.Elm.newStr();
    static final Proto.Elm $$T_Obj = Proto.Elm.newObj();

    static final Proto.Fxn $$T_Met = new Proto.Fxn(null, null, 0, -1, false);
    static final Proto.Map $$T_Map = new Proto.Map($$T_Obj);
    static final Proto.Arr $$T_Vec = new Proto.Arr($$T_Obj);

    static final XScriptO $$DEFAULT = Value.DEFAULT;
    static final Object $$UNDEF = Undefined.instance;

    static final Proto.Obj $$Package = (Proto.Obj)Global.get("$$Package");
    static final Proto.Obj $$Module = (Proto.Obj)Global.get("$$Module");
    static final Proto.Obj $$Instance = (Proto.Obj)Global.get("$$Instance");
    static final Proto.Obj $$Params = (Proto.Obj)Global.get("$$Params");

    static final Object $$objFldGet = Global.get("$$objFldGet");
    static final Object $$objFldSet = Global.get("$$objFldSet");
    static final Object $$proxyGet = Global.get("$$proxyGet");
    static final Object $$proxySet = Global.get("$$proxySet");
    static final Object $$delegGet = Global.get("$$delegGet");
    static final Object $$delegSet = Global.get("$$delegSet");

    Scriptable xdcO;
    Session ses;
    Value.Obj om;

    boolean isROV;
    boolean isCFG;

    Proto.Obj pkgP;
    Value.Obj pkgV;

    ArrayList<Object> imports = new ArrayList<Object>();
    ArrayList<Object> loggables = new ArrayList<Object>();
    ArrayList<Object> mcfgs = new ArrayList<Object>();
    ArrayList<Object> proxies = new ArrayList<Object>();
    ArrayList<Object> sizes = new ArrayList<Object>();
    ArrayList<Object> tdefs = new ArrayList<Object>();

    void $$IMPORTS()
    {
        Global.callFxn("loadPackage", xdcO, "ti.sdo.ce.universal");
        Global.callFxn("loadPackage", xdcO, "gsoc.algos.cvDFT");
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("gsoc.algos.cvDFT.ce.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("gsoc.algos.cvDFT.ce", new Value.Obj("gsoc.algos.cvDFT.ce", pkgP));
    }

    void CVDFT$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("gsoc.algos.cvDFT.ce.CVDFT.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("gsoc.algos.cvDFT.ce.CVDFT", new Value.Obj("gsoc.algos.cvDFT.ce.CVDFT", po));
        pkgV.bind("CVDFT", vo);
        // decls 
    }

    void CVDFT$$CONSTS()
    {
        // module CVDFT
    }

    void CVDFT$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void CVDFT$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void CVDFT$$SIZES()
    {
    }

    void CVDFT$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "gsoc/algos/cvDFT/ce/CVDFT.xs");
        om.bind("gsoc.algos.cvDFT.ce.CVDFT$$capsule", cap);
        po = (Proto.Obj)om.findStrict("gsoc.algos.cvDFT.ce.CVDFT.Module", "gsoc.algos.cvDFT.ce");
        po.init("gsoc.algos.cvDFT.ce.CVDFT.Module", om.findStrict("ti.sdo.ce.universal.IUNIVERSAL.Module", "gsoc.algos.cvDFT.ce"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("alg", (Proto)om.findStrict("gsoc.algos.cvDFT.CVDFT.Module", "gsoc.algos.cvDFT.ce"), om.find("gsoc.algos.cvDFT.CVDFT"), "rh");
                po.addFld("ialgFxns", $$T_Str, "CVDFT_GSOC_IALG", "rh");
                fxn = Global.get(cap, "module$use");
                if (fxn != null) om.bind("gsoc.algos.cvDFT.ce.CVDFT$$module$use", true);
                if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
                fxn = Global.get(cap, "module$meta$init");
                if (fxn != null) om.bind("gsoc.algos.cvDFT.ce.CVDFT$$module$meta$init", true);
                if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
                fxn = Global.get(cap, "module$validate");
                if (fxn != null) om.bind("gsoc.algos.cvDFT.ce.CVDFT$$module$validate", true);
                if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "getCreationStackSize");
                if (fxn != null) po.addFxn("getCreationStackSize", (Proto.Fxn)om.findStrict("ti.sdo.ce.ICodec$$getCreationStackSize", "gsoc.algos.cvDFT.ce"), fxn);
                fxn = Global.get(cap, "getDaramScratchSize");
                if (fxn != null) po.addFxn("getDaramScratchSize", (Proto.Fxn)om.findStrict("ti.sdo.ce.ICodec$$getDaramScratchSize", "gsoc.algos.cvDFT.ce"), fxn);
                fxn = Global.get(cap, "getSaramScratchSize");
                if (fxn != null) po.addFxn("getSaramScratchSize", (Proto.Fxn)om.findStrict("ti.sdo.ce.ICodec$$getSaramScratchSize", "gsoc.algos.cvDFT.ce"), fxn);
                fxn = Global.get(cap, "getStackSize");
                if (fxn != null) po.addFxn("getStackSize", (Proto.Fxn)om.findStrict("ti.sdo.ce.ICodec$$getStackSize", "gsoc.algos.cvDFT.ce"), fxn);
                fxn = Global.get(cap, "getUUID");
                if (fxn != null) po.addFxn("getUUID", (Proto.Fxn)om.findStrict("ti.sdo.ce.ICodec$$getUUID", "gsoc.algos.cvDFT.ce"), fxn);
    }

    void CVDFT$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("gsoc.algos.cvDFT.ce.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "gsoc.algos.cvDFT.ce"));
        pkgP.bind("$capsule", $$UNDEF);
        pkgV.init2(pkgP, "gsoc.algos.cvDFT.ce", Value.DEFAULT, false);
        pkgV.bind("$name", "gsoc.algos.cvDFT.ce");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "gsoc.algos.cvDFT.ce.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("ti.sdo.ce.universal", Global.newArray()));
        imports.add(Global.newArray("gsoc.algos.cvDFT", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['gsoc.algos.cvDFT.ce'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        sb.append("if('suffix' in xdc.om['xdc.IPackage$$LibDesc']) {\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void CVDFT$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("gsoc.algos.cvDFT.ce.CVDFT", "gsoc.algos.cvDFT.ce");
        po = (Proto.Obj)om.findStrict("gsoc.algos.cvDFT.ce.CVDFT.Module", "gsoc.algos.cvDFT.ce");
        vo.init2(po, "gsoc.algos.cvDFT.ce.CVDFT", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("gsoc.algos.cvDFT.ce.CVDFT$$capsule", "gsoc.algos.cvDFT.ce"));
        vo.bind("$package", om.findStrict("gsoc.algos.cvDFT.ce", "gsoc.algos.cvDFT.ce"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "gsoc.algos.cvDFT.ce")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("gsoc.algos.cvDFT.ce.CVDFT$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        pkgV.bind("CVDFT", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("CVDFT");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("gsoc.algos.cvDFT.ce.CVDFT", "gsoc.algos.cvDFT.ce"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("gsoc.algos.cvDFT.ce.CVDFT")).bless();
        ((Value.Arr)om.findStrict("$packages", "gsoc.algos.cvDFT.ce")).add(pkgV);
    }

    public void exec( Scriptable xdcO, Session ses )
    {
        this.xdcO = xdcO;
        this.ses = ses;
        om = (Value.Obj)xdcO.get("om", null);

        Object o = om.geto("$name");
        String s = o instanceof String ? (String)o : null;
        isCFG = s != null && s.equals("cfg");
        isROV = s != null && s.equals("rov");

        $$IMPORTS();
        $$OBJECTS();
        CVDFT$$OBJECTS();
        CVDFT$$CONSTS();
        CVDFT$$CREATES();
        CVDFT$$FUNCTIONS();
        CVDFT$$SIZES();
        CVDFT$$TYPES();
        if (isROV) {
            CVDFT$$ROV();
        }//isROV
        $$SINGLETONS();
        CVDFT$$SINGLETONS();
        $$INITIALIZATION();
    }
}
