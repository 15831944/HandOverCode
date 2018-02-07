GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CardTestN]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[CardTestN]
GO
--CardTestN ���뿨��������
CREATE PROCEDURE CardTestN  
WITH   ENCRYPTION 
AS
SET NOCOUNT ON --�����ؼ���
Set ARITHABORT ON
SELECT [dbo].charDecode(׼��֤�����) as ׼��֤�����,������,[dbo].charDecode(����) as MSG0,�Ա� as MSG1,[dbo].charDecode(��ˮ��) as MSG2,[dbo].charDecode(���֤�����) as MSG3,��У���� as MSG4,����Ա1 as MSG5,���մ��� as MSG6,������+'-'+SysCfg.��ע+'-'+���Գ��� as MSG7
FROM StudentInfo 
LEFT JOIN SchoolInfo ON ������=��У��� 
JOIN SysCfg ON ������=��Ŀ
WHERE ��¼״̬='�ѵ�¼' 
AND StudentInfo.״̬='3' 
AND (Select CONVERT(varchar(100), [dbo].dateDecode(Ԥ������), 23)) = (Select CONVERT(varchar(100), GETDATE(), 23))
/*--2014-07-07 ZSZ*/

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[pr_ErrorRecordALL]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[pr_ErrorRecordALL]
GO
--pr_ErrorRecordALL ���뿨��������
CREATE PROCEDURE pr_ErrorRecordALL
(
	@ZKZM_Number varchar(50)
) 
WITH   ENCRYPTION 
AS
SET NOCOUNT ON --�����ؼ���
Set ARITHABORT ON
declare @zkzmbhbase64 varchar(50)
select @zkzmbhbase64=[dbo].charEncode(@ZKZM_Number)
SELECT e.��¼���,e.������,CONVERT(varchar(10),[dbo].dateDecode(e.����ʱ��),8) as ����ʱ��,[dbo].charDecode(ed.�۷�����) as �۷�����,[dbo].intDecode(ed.�۳�����) as �۳����� FROM ErrorRecords e,StudentInfo s,ErrorData ed
WHERE e.׼��֤�����=s.׼��֤����� 
AND e.������=ed.������
AND e.���Դ���=s.���Դ��� 
AND e.���մ���=s.���մ��� 
AND e.׼��֤�����=@zkzmbhbase64
ORDER BY ��¼��� ASC
/*2014-07-08 ZSZ ����:׼��֤����� */

GO
--����pr_ErrorRecords �����ڵ�ʱ��
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[pr_ErrorRecords]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
begin
exec('CREATE PROCEDURE pr_ErrorRecords
(
	@ZKZM_Number varchar(50),--׼��֤�����
	@Ejlbh bigint, --�����¼���
	@intErrorCode int OUTPUT
) 
WITH   ENCRYPTION 
AS
BEGIN
SET NOCOUNT ON --�����ؼ���
set @intErrorCode=-1
return @intErrorCode
END
')
end
GO
--�޸� pr_ErrorRecords 2014-10-09 �ӵ��մ����ж� 
ALTER PROCEDURE pr_ErrorRecords
(
	@ZKZM_Number varchar(50),--׼��֤�����
	@Ejlbh bigint, --�����¼���
	@intErrorCode int OUTPUT
) 
WITH   ENCRYPTION 
AS
BEGIN
SET NOCOUNT ON --�����ؼ���
Set ARITHABORT ON
DECLARE @Status char(1)--״̬
declare @KscsN tinyint --���Դ���
declare @DrcsN tinyint --���մ���
declare @KSCJ int --�ɼ�
declare @ErrorCode int --������
declare @ErrorCodejl int --��¼���
declare @zkzmbhbase64 varchar(50)
SELECT @zkzmbhbase64=[dbo].charEncode(@ZKZM_Number)
SELECT @Status=״̬,@KscsN=���Դ���,@DrcsN=���մ��� FROM Studentinfo WHERE ׼��֤�����=@zkzmbhbase64
SELECT @KSCJ=100-SUM([dbo].intDecode(�۳�����)) FROM ErrorRecords JOIN ErrorData ON  ErrorRecords.������=ErrorData.������ 
WHERE ErrorRecords.׼��֤�����=@zkzmbhbase64 and ���Դ���=@KscsN and ���մ���=@DrcsN

set @intErrorCode = 0
if exists(Select TOP 1 ������ from ErrorRecords where ׼��֤�����=@zkzmbhbase64 and ��¼���>@Ejlbh and ���մ���=@DrcsN and ���Դ���=@KscsN)
begin --1b
Select TOP 1 @ErrorCode=������,@ErrorCodejl=��¼��� from ErrorRecords where ׼��֤�����=@zkzmbhbase64 and ��¼���>@Ejlbh and ���մ���=@DrcsN and ���Դ���=@KscsN
if exists(select * from errordata where �۳�����!=[dbo].intEncode(0, RAND()) and ������=@ErrorCode) 
BEGIN --2b
declare @counterrline int --ͳ�ƴ�����
SELECT @counterrline=Count(*) FROM ErrorRecords er JOIN ErrorData ed ON  er.������=ed.������ 
WHERE er.׼��֤�����=@zkzmbhbase64 and ���Դ���=@KscsN and ���մ���=@DrcsN and ed.�۳�����!=[dbo].intEncode(0, RAND()) and er.��¼���<=@Ejlbh

Select er.��¼���,[dbo].charDecode(ed.�۷�����) as �۷�����,CONVERT(varchar(10),[dbo].dateDecode(er.����ʱ��),8) as ����ʱ��,[dbo].intDecode(ed.�۳�����) as �۳�����,�ɼ�=@KSCJ,ELine=@counterrline
from ErrorRecords er JOIN ErrorData ed ON er.������=ed.������ 
where er.��¼���=@ErrorCodejl

set @intErrorCode = 3
goto ERR_HANDLER
END /*17C53*/ --2e
else
BEGIN --3b
if (@ErrorCode = 201990 OR @ErrorCode=204990 OR @ErrorCode=203990 OR @ErrorCode=206990 OR @ErrorCode=207990 OR @ErrorCode=215990 OR @ErrorCode=216990 )
begin --4b
Select ErrorRecords.��¼���,ErrorRecords.������,[dbo].charDecode(ErrorData.�۷�����) as �۷�����
from ErrorRecords JOIN ErrorData ON ErrorRecords.������=ErrorData.������ 
where ErrorRecords.��¼���=@ErrorCodejl

set @intErrorCode = 5
goto ERR_HANDLER
end --4e
else if((@ErrorCode>201500 and  @ErrorCode<201700) OR (@ErrorCode>203500 and  @ErrorCode<203700) OR (@ErrorCode>204500 and  @ErrorCode<204700)OR (@ErrorCode>206500 and  @ErrorCode<206700) OR (@ErrorCode>207500 and  @ErrorCode<207700)
OR (@ErrorCode>214500 and  @ErrorCode<214700)OR (@ErrorCode>215500 and  @ErrorCode<215700) OR (@ErrorCode>216500 and  @ErrorCode<216700) OR @ErrorCode=10001)
begin --5b
Select er.��¼���,ed.������,[dbo].charDecode(ed.�۷�����) as �۷�����
from ErrorRecords er JOIN ErrorData ed ON er.������=ed.������ 
where er.��¼���=@ErrorCodejl
set @intErrorCode = 2
goto ERR_HANDLER
end --5e
else
begin --6b
set @intErrorCode = 0
goto ERR_HANDLER
end --6e
END /*Not 17C53*/ --3e
end --1e
else
begin --7b
if(@Status =1 OR @Status =2)
BEGIN
	Select ״̬=@Status,�ɼ�=@KSCJ,S=127,E=127
	Set @intErrorCode =6
	goto ERR_HANDLER
END
else
BEGIN --8 zt!=1 or zt!=2
	Select ״̬=@Status
	Set @intErrorCode=-1
END --8 zt!=1 or zt!=2
end	--7e

ERR_HANDLER:
	return @intErrorCode
END
/*2017-10-02 ZSZ ����:׼��֤�����,���¼���,����ֵ*/
GO
