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

public class gsoc_algos_cvDFT
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
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("gsoc.algos.cvDFT.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("gsoc.algos.cvDFT", new Value.Obj("gsoc.algos.cvDFT", pkgP));
    }

    void CVDFT$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("gsoc.algos.cvDFT.CVDFT.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("gsoc.algos.cvDFT.CVDFT", new Value.Obj("gsoc.algos.cvDFT.CVDFT", po));
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

        po = (Proto.Obj)om.findStrict("gsoc.algos.cvDFT.CVDFT.Module", "gsoc.algos.cvDFT");
        po.init("gsoc.algos.cvDFT.CVDFT.Module", $$Module);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("watermark", $$T_Bool, true, "wh");
                po.addFld("codeSection", $$T_Str, $$UNDEF, "wh");
                po.addFld("udataSection", $$T_Str, $$UNDEF, "wh");
                po.addFld("dataSection", $$T_Str, $$UNDEF, "wh");
    }

    void CVDFT$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("gsoc.algos.cvDFT.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "gsoc.algos.cvDFT"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "gsoc/algos/cvDFT/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "gsoc.algos.cvDFT"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "gsoc.algos.cvDFT"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "gsoc.algos.cvDFT"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "gsoc.algos.cvDFT"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "gsoc.algos.cvDFT"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "gsoc.algos.cvDFT"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "gsoc.algos.cvDFT", Value.DEFAULT, false);
        pkgV.bind("$name", "gsoc.algos.cvDFT");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "gsoc.algos.cvDFT.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['gsoc.algos.cvDFT'];\n");
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

        vo = (Value.Obj)om.findStrict("gsoc.algos.cvDFT.CVDFT", "gsoc.algos.cvDFT");
        po = (Proto.Obj)om.findStrict("gsoc.algos.cvDFT.CVDFT.Module", "gsoc.algos.cvDFT");
        vo.init2(po, "gsoc.algos.cvDFT.CVDFT", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("$package", om.findStrict("gsoc.algos.cvDFT", "gsoc.algos.cvDFT"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "gsoc.algos.cvDFT")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("gsoc.algos.cvDFT.CVDFT$$instance$static$init", null) ? 1 : 0);
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
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("gsoc.algos.cvDFT.CVDFT", "gsoc.algos.cvDFT"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("gsoc.algos.cvDFT.CVDFT")).bless();
        ((Value.Arr)om.findStrict("$packages", "gsoc.algos.cvDFT")).add(pkgV);
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
